#include "gfxutils.h"

#include <tice.h>
#include <string.h>
#include <limits.h>

#define isbuf (gfx_GetDraw() == gfx_buffer)
#define doblit() \
    if (isbuf)   \
        gfx_BlitBuffer();

static uint8_t foreground = 0, background = 255;
static uint8_t fgbak, bgbak, cbak;

/*static*/ const gfx_sprite_t *bgsprite;

static sk_key_t getk()
{
    sk_key_t k;
    while (!(k = os_GetCSC()))
        ;
    return k;
}

static void setupcolors()
{
    gfx_SetTextTransparentColor(background);
    bgbak = gfx_SetTextBGColor(background);
    fgbak = gfx_SetTextFGColor(foreground);
}

static void restorecolors()
{
    gfx_SetTextTransparentColor(bgbak);
    gfx_SetTextBGColor(bgbak);
    gfx_SetTextFGColor(fgbak);
}

static void setcol(uint8_t color)
{
    cbak = gfx_SetColor(color);
}

static void restcol()
{
    gfx_SetColor(cbak);
}

static const char *chars[] = {
    // normal
    "+-*/^\0\0-369)\0\0\0.258(\0\0\0000147,\0\0X\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
    // 2nd
    "\0][e\0\0\0\0\0\0w}\0\0\0i\0\0v{\0\0\0\0\0\0u\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
    // alpha
    "\"WRMH\0\0?\0VQLG\0\0:ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0"};

static char lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

uint16_t gfxutils_GetTextInput(const char *title, char *buffer, uint16_t length)
{
    int16_t width, x;
    uint16_t titlew, ind;
    const uint8_t height = 16 + 8 + 10, y = (LCD_HEIGHT - height) / 2;
    uint8_t spmode; // 0=normal, 1=2nd, 2=alpha-lwr, 3=alpha, 5=alpha-lwr+2nd, 9=alpha+2nd

    width = titlew = gfx_GetStringWidth(title) * 2;
    if (length * 8 > width)
        width = length * 8;

    width += 10;
    x = (LCD_WIDTH - width) / 2;

    setupcolors();
    setcol(background);
    ind = 0;
    buffer[0] = 0;
    spmode = 0;
    for (;;)
    {
        gfx_FillRectangle(x, y, width, height);

        gfx_SetTextScale(2, 2);
        gfx_PrintStringXY(title, (LCD_WIDTH - titlew) / 2, y + 3);

        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY(buffer, (LCD_WIDTH - gfx_GetStringWidth(buffer)) / 2, y + 3 + 16 + 5);
        doblit();

        sk_key_t k = getk();
        if (k == sk_Enter)
            break;
        switch (k)
        {
        case sk_Del:
            if (ind)
                buffer[--ind] = 0;
            break;
        case sk_Clear:
            ind = 0;
            buffer[0] = 0;
            break;
        case sk_2nd:
            switch (spmode)
            {
            case 0:
            case 1:
                spmode = 1 - spmode;
                break;
            case 5:
                spmode = 2;
                break;
            case 9:
                spmode = 3;
                break;
            }
            break;
        case sk_Alpha:
            switch (spmode)
            {
            case 0:
            case 1:
                spmode = 3;
                break;
            case 2:
            case 5:
                spmode = 0;
                break;
            case 3:
            case 9:
                spmode = 2;
                break;
            }
            break;
        default:
            if (ind == length)
                break;
            spmode %= 4;
            if (spmode == 2)
                buffer[ind] = lower(chars[2][k - 10]);
            else if (spmode == 3)
                buffer[ind] = chars[2][k - 10];
            else
                buffer[ind] = chars[spmode][k - 10];
            if (buffer[ind])
                buffer[++ind] = 0;
            break;
        }
    }
    restorecolors();
    restcol();
    return ind;
}

uint8_t gfxutils_SetBackgroundColor(uint8_t color)
{
    background = color;
    gfx_SetTextTransparentColor(color);
    return gfx_SetTextBGColor(color);
}

uint8_t gfxutils_SetForegroundColor(uint8_t color)
{
    foreground = color;
    return gfx_SetTextFGColor(color);
}

const gfx_sprite_t *gfxutils_SetBackgroundSprite(const gfx_sprite_t *sprite)
{
    const gfx_sprite_t *s;
    s = bgsprite;
    bgsprite = sprite;
    return s;
}

uint8_t gfxutils_FullScreenSprite(const gfx_sprite_t *sprite)
{
    uint8_t scale = LCD_WIDTH / sprite->width;
    if (LCD_HEIGHT / sprite->height < scale)
        scale = LCD_HEIGHT / sprite->height;
    gfx_ScaledSprite_NoClip(sprite, (LCD_WIDTH - sprite->width * scale) / 2, (LCD_HEIGHT - sprite->height * scale) / 2, scale, scale);
    return scale;
}

void gfxutils_ClearScreen(void)
{
    gfx_FillScreen(background);
    if (bgsprite != NULL)
        gfxutils_FullScreenSprite(bgsprite);
    if (isbuf)
        gfx_BlitBuffer();
}

uint8_t gfxutils_FindClosestColor(uint8_t r, uint8_t g, uint8_t b)
{
    unsigned int mindiff = 1U + INT_MAX + INT_MAX;
    uint8_t col;
    uint8_t i = 0;
    do
    {
        // unsigned int dif = rd * rd + gd * gd + bd * bd;
        gfxutils_rgb888_t c = gfxutils_565To888(gfx_palette[i]);
        unsigned int rd = r - c.r, gd = g - c.g, bd = b - c.b;
        unsigned int dif = rd * rd + gd * gd + bd * bd;
        if (dif < mindiff)
        {
            mindiff = dif;
            col = i;
        }
        i++;
    } while (i);
    return col;
}

// rgb565: rrrrrggg gggbbbbb
gfxutils_rgb888_t gfxutils_565To888(uint16_t rgb565)
{
    gfxutils_rgb888_t ret;
    ret.r = ((uint8_t)(rgb565 >> 8)) & 0b11111000;
    ret.g = ((uint8_t)(rgb565 >> 3)) & 0b11111100;
    ret.b = ((uint8_t)(rgb565 << 3)) & 0b11111000;
    return ret;
}
