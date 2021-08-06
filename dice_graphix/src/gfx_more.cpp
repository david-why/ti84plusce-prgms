#include <graphx.h>
#include <tice.h>
#include <string.h>

#ifndef uint24_t
typedef unsigned int uint24_t;
#endif /* uint24_t */

typedef struct pos_t
{
    int x;
    int16_t y;

    pos_t(int x, uint16_t y) : x(x), y(y) {}
    pos_t() {}
} pos_t;

/**
 * Colors a sprite. 
 * 
 * This function sets ALL pixels in the sprite, except for those with color 
 * @p keep, to @p color. 
 * You have been warned. 
 * 
 * @param sprite The sprite to color.
 * @param color The palette index of the color to set.
 * @param keep The color to not color in. Might be the transparent color.
 */
void gfx_ColorSprite(gfx_sprite_t *sprite, uint8_t color, uint16_t keep = 256)
{
    for (uint16_t i = 0; i < sprite->width * sprite->height; i++)
    {
        if (sprite->data[i] != keep)
        {
            sprite->data[i] = color;
        }
    }
}

/**
 * Centers a scaled sprite. 
 * 
 * Scaling factors must be greater than or equal to 1, and an integer factor. 
 * Sprites are scaled by multiplying the dimensions by the respective factors. 
 * 
 * @param sprite Pointer to an initialized sprite structure.
 * @param buff Buffer to store the scaled sprite.
 * @param width_scale Width scaling factor.
 * @param height_scale Height scaling factor.
 * @returns The position of the drawn sprite, in (x, y) pair.
 */
pos_t gfx_CenteredScaledSprite(const gfx_sprite_t *sprite, gfx_sprite_t *buff,
                               uint8_t width_scale,
                               uint8_t height_scale)
{
    uint16_t width = sprite->width * width_scale;
    uint16_t height = sprite->height * height_scale;
    int x;
    uint16_t y;
    buff->width = width;
    buff->height = height;

    x = (LCD_WIDTH - width) / 2;
    y = (LCD_HEIGHT - height) / 2;

    gfx_ScaleSprite(sprite, buff);
    gfx_Sprite(buff, x, y);

    free(&width);
    free(&height);
    free(&x);
    free(&y);

    return {x, y};
}
/**
 * Centers a scaled sprite. 
 * 
 * Scaling factors must be greater than or equal to 1, and an integer factor. 
 * Sprites are scaled by multiplying the dimensions by the respective factors. 
 * 
 * @param sprite Pointer to an initialized sprite structure.
 * @param width_scale Width scaling factor.
 * @param height_scale Height scaling factor.
 * @returns The position of the drawn sprite, in (x, y) pair.
 */
pos_t gfx_CenteredScaledSprite(const gfx_sprite_t *sprite,
                               uint8_t width_scale,
                               uint8_t height_scale)
{
    gfx_sprite_t *draw = gfx_MallocSprite(sprite->width * width_scale, sprite->height * height_scale);
    pos_t ret = gfx_CenteredScaledSprite(sprite, draw, width_scale, height_scale);
    free(draw);
    free(&draw);
    return ret;
}

/**
 * Prints a string at a specific location. 
 *
 * Scaling factor must be greater than or equal to 1, and an integer factor. 
 * 
 * Outputs a string at the supplied coordinates. 
 * Position is measured from top left origin of screen. 
 * @param string Pointer to string to print.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param scale Scaling factor.
 */
void gfx_PrintScaledStringXY(const char *string,
                             int x, int y,
                             double scale,
                             uint16_t color = 256,
                             uint16_t keep = 256)
{
    const char *ptr;
    uint16_t scaled = scale * 8;
    gfx_sprite_t *chr = gfx_MallocSprite(scaled, scaled);

    for (ptr = string; *ptr != '\0'; ptr++)
    {
        gfx_ScaleSprite(gfx_GetSpriteChar(*ptr), chr);
        if (color < 256)
        {
            gfx_ColorSprite(chr, color, keep);
        }
        gfx_Sprite((gfx_sprite_t *)chr, x, y);
        x += scaled;
    }

    free(&ptr);
    free(&scaled);
    free(&chr);
}

/**
 * Prints a centered string at a specific y coordinate. 
 * 
 * @param string Pointer to string to print.
 * @param y Y coordinate.
 * @note By default, no text clipping is performed. See gfx_SetTextConfig.
 */
void gfx_PrintCenteredString(const char *string, int y)
{
    gfx_PrintStringXY(string, (LCD_WIDTH - gfx_GetStringWidth(string)) / 2, y);
}

/**
 * Prints a centered and scaled string at a specific y coordinate. 
 * 
 * @param string Pointer to string to print.
 * @param y Y coordinate.
 * @param scale Scaling factor.
 */
#define gfx_PrintCenteredScaledString(string, y, scale, color, bgcolor) \
    gfx_PrintScaledStringXY((string), (LCD_WIDTH - gfx_GetStringWidth(string) * (scale)) / 2, (y), (scale), (color), (bgcolor))

/**
 * Resizes a sprite to new dimensions.
 *
 * @param sprite_in Input sprite to scale.
 * @param sprite_out Pointer to where scaled sprite will be stored.
 * @param width_scale Width scaling factor.
 * @param height_scale Height scaling factor.
 * @returns A pointer to \p sprite_out.
 */
gfx_sprite_t *gfx_ScaleSprite(const gfx_sprite_t *sprite_in, gfx_sprite_t *sprite_out,
                              uint8_t width_scale, uint8_t height_scale)
{
    sprite_out->width = sprite_in->width * width_scale;
    sprite_out->height = sprite_in->height * height_scale;

    uint8_t data;
    for (int x = 0; x < sprite_in->width; x++)
    {
        for (int y = 0; y < sprite_in->height; y++)
        {
            data = sprite_in->data[y * sprite_in->width + x];
            for (int xnow = x * width_scale; xnow < x * width_scale + width_scale; xnow++)
            {
                for (int ynow = y * height_scale; ynow < y * height_scale + height_scale; ynow++)
                {
                    sprite_out->data[ynow * sprite_out->width + xnow] = data;
                }
            }
        }
    }

    free(&data);

    return sprite_out;
}

/**
 * Draws a scaled sprite.
 *
 * Scaling factors must be greater than or equal to 1, and an integer factor.
 * Sprites are scaled by multiplying the dimensions by the respective factors.
 *
 * @warning
 * If used outside of a function body, the memory will be allocated in the
 * global uninitialized data segment (BSS). If used inside a function body, the
 * memory will be allocated on the stack. If the sprite is sufficiently large,
 * usage inside a function body will overflow the stack, so it is recommended
 * that this normally be used outside of a function body.
 *
 * @param sprite Pointer to an initialized sprite structure.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param width_scale Width scaling factor.
 * @param height_scale Height scaling factor.
 */
void gfx_ScaledSprite(const gfx_sprite_t *sprite,
                      uint24_t x, uint8_t y,
                      uint8_t width_scale, uint8_t height_scale)
{
    uint16_t width = sprite->width * width_scale;
    uint16_t height = sprite->height * height_scale;
    uint8_t *scaled_sprite_data[2 + width * height];
    gfx_sprite_t *scaled_sprite = (gfx_sprite_t *)scaled_sprite_data;
    free(&scaled_sprite_data);
    scaled_sprite->width = width;
    scaled_sprite->height = height;
    gfx_ScaleSprite(sprite, scaled_sprite);
    gfx_Sprite(scaled_sprite, x, y);
    free(&width);
    free(&height);
    free(scaled_sprite);
    free(&scaled_sprite);
}

#define gfx_ScaledSprite _Pragma("GCC warning \"'gfx_ScaledSprite might cause RAM resets! Use at your own risk.\"") gfx_ScaledSprite
