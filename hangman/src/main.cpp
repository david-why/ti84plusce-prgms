#include <string.h>
#include <stdio.h>

#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

#ifndef uint24_t
typedef unsigned int uint24_t;
#endif

#define WORDLENGTH 11
#define KEYWORDSLENGTH 1000

typedef struct
{
    uint8_t length;
    char word[WORDLENGTH];
} keyword_t;

typedef struct
{
    uint16_t testbytes;
    uint16_t length;
    keyword_t words[KEYWORDSLENGTH];
} keywords_t;

#define gfx_WeightedCircle_NoClip(x, y, radius, thickness)       \
    do                                                           \
    {                                                            \
        gfx_FillCircle_NoClip((x), (y), (radius));               \
        uint8_t __color = gfx_SetColor(255);                     \
        gfx_FillCircle_NoClip((x), (y), (radius) - (thickness)); \
        gfx_SetColor(__color);                                   \
    } while (0)

#define LIVES 7
uint8_t lostlives;
bool revealed[WORDLENGTH];
keywords_t *kwds;
uint8_t word;
uint8_t letter;
#define MAXLETTERS 8
bool missed[26];
bool matched[26];
uint8_t hints = 0;
uint8_t cheat;
#define CHEATED 5
const char chars[] = "           WRMH " // 0x
                     "   VQLG   ZUPKFC" // 1x
                     "  YTOJEB  XSNIDA" // 2x
    ;
char strbuf[100];

char packs[10][9];

void draw_Noose()
{
    gfx_SetColor(0);
    gfx_SetTextFGColor(0);
    gfx_Rectangle_NoClip(2, 2, LCD_WIDTH - 4, 14);
    gfx_PrintStringXY("Hangman game by David [mode]=quit [clear]=new", 5, 5);
    gfx_FillRectangle_NoClip(10, 220, 100, 5);
    gfx_FillRectangle_NoClip(32, 30, 5, 190);
    gfx_FillRectangle_NoClip(32, 30, 50, 5);
    gfx_FillRectangle_NoClip(78, 30, 5, 50);
}

#define LOSTLIVES(n) if (lostlives >= (n))

void draw_Hangman(uint8_t xo = 0, uint8_t yo = 0)
{
    gfx_SetColor(255);
    uint8_t tmp = gfx_SetTransparentColor(0);
    gfx_FillRectangle_NoClip(240, 18, 80, 8);
    gfx_SetTransparentColor(tmp);
    gfx_SetTextFGColor(0);
    sprintf(strbuf, "Hints: %u", hints);
    gfx_PrintStringXY(strbuf, 240, 18);
    gfx_SetColor(0);
    LOSTLIVES(1)
    {
        gfx_WeightedCircle_NoClip(80 + xo, 80 + yo, 25, 5);
    }
    LOSTLIVES(2)
    {
        gfx_FillCircle_NoClip(73 + xo, 74 + yo, 2);
        gfx_FillCircle_NoClip(87 + xo, 74 + yo, 2);
        gfx_WeightedCircle_NoClip(80 + xo, 89 + yo, 7, 3);
    }
    LOSTLIVES(3)
    {
        gfx_FillRectangle_NoClip(78 + xo, 104 + yo, 5, 57);
    }
    LOSTLIVES(4)
    {
        gfx_FillTriangle_NoClip(81 + xo, 126 + yo, 56 + xo, 151 + yo, 53 + xo, 148 + yo);
        gfx_FillTriangle_NoClip(81 + xo, 126 + yo, 78 + xo, 123 + yo, 53 + xo, 148 + yo);
    }
    LOSTLIVES(5)
    {
        gfx_FillTriangle_NoClip(81 + xo, 123 + yo, 107 + xo, 148 + yo, 104 + xo, 151 + yo);
        gfx_FillTriangle_NoClip(81 + xo, 123 + yo, 78 + xo, 126 + yo, 104 + xo, 151 + yo);
    }
    LOSTLIVES(6)
    {
        gfx_FillTriangle_NoClip(81 + xo, 161 + yo, 56 + xo, 186 + yo, 53 + xo, 183 + yo);
        gfx_FillTriangle_NoClip(81 + xo, 161 + yo, 78 + xo, 158 + yo, 53 + xo, 183 + yo);
    }
    LOSTLIVES(7)
    {
        gfx_FillTriangle_NoClip(81 + xo, 158 + yo, 107 + xo, 183 + yo, 104 + xo, 186 + yo);
        gfx_FillTriangle_NoClip(81 + xo, 158 + yo, 78 + xo, 161 + yo, 104 + xo, 186 + yo);
    }
}

void draw_Missed()
{
    gfx_SetTextFGColor(224);
    for (uint8_t i = 0; i < sizeof(missed); i++)
    {
        if (missed[i])
        {
            gfx_sprite_t *chr = gfx_GetSpriteChar('A' + i);
            uint16_t x = 120 + (i % 7) * 20;
            uint8_t y = 30 + (i / 7) * 20;
            gfx_ScaledSprite_NoClip(chr, x, y, 2, 2);
        }
    }
}

void draw_Word(uint8_t color = 0)
{
    gfx_SetTextFGColor(color);
    keyword_t &kw = kwds->words[word];
    for (uint8_t i = 0; i < kw.length; i++)
    {
        uint16_t x = 120 + (i % 9) * 20;
        uint8_t y = 180 + (i / 9) * 20;
        gfx_sprite_t *chr;
        if (revealed[i])
            chr = gfx_GetSpriteChar(kw.word[i]);
        else
        {
            if (letter == MAXLETTERS)
            {
                gfx_SetTextFGColor(222);
                chr = gfx_GetSpriteChar(kw.word[i]);
                gfx_SetTextFGColor(color);
            }
            else
            {
                chr = gfx_GetSpriteChar('_');
            }
        }
        gfx_ScaledSprite_NoClip(chr, x, y, 2, 2);
    }
}

void entered(char c)
{
    char *ptr = strchr(kwds->words[word].word, c);
    if (ptr == NULL)
    {
        missed[c - 'A'] = true;
        lostlives++;
    }
    for (uint8_t i = 0; i < kwds->words[word].length; i++)
    {
        if (kwds->words[word].word[i] == c)
        {
            matched[c - 'A'] = true;
            revealed[i] = true;
        }
    }
}

#undef LOSTLIVES

void iamdead()
{
    gfx_FillRectangle_NoClip(69, 70, 3, 3);
    gfx_FillRectangle_NoClip(75, 70, 3, 3);
    gfx_FillRectangle_NoClip(69, 76, 3, 3);
    gfx_FillRectangle_NoClip(75, 76, 3, 3);
    gfx_FillRectangle_NoClip(83, 70, 3, 3);
    gfx_FillRectangle_NoClip(89, 70, 3, 3);
    gfx_FillRectangle_NoClip(83, 76, 3, 3);
    gfx_FillRectangle_NoClip(89, 76, 3, 3);
}

void iwon()
{
    gfx_SetColor(255);
    gfx_FillRectangle_NoClip(53, 55, 55, 131);
    lostlives = LIVES;
    draw_Hangman(0, 38);
}

int main()
{
    char *name;
    uint8_t *search_pos = NULL;
    uint8_t pack = 0;
    while ((name = ti_Detect((void **)&search_pos, NULL)) != NULL)
    {
        int archived = 1;
        var_t *v = os_GetAppVarData(name, &archived);
        if (!archived || !v->size)
            continue;
        uint16_t c = *(uint16_t *)v->data;
        if (c != 12345)
            continue;
        strcpy(packs[pack++], name);
    }

    if (pack == 0)
    {
        os_ClrHome();
        os_PutStrFull("You have not sent the     "
                      "keywords. Please send the "
                      "AppVar HNGMNWRD.8xv to the"
                      "calc and try again.");
        while (!os_GetCSC())
            delay(10);
        return 1;
    }

    srandom(rtc_Time());
    gfx_Begin();
    gfx_SetTransparentColor(255);

    var_t *keywords;
    if (pack == 1)
    {
        keywords = os_GetAppVarData("HNGMNWRD", NULL);
    }
    else
    {
        gfx_PrintStringXY("Select pack:", 0, 0);
        gfx_SetTextFGColor(0);
        for (uint8_t p = 0; p < pack; p++)
        {
            gfx_PrintStringXY(packs[p], 16, 10 * (p + 1));
        }
        uint8_t selected = 0;
        sk_key_t key = 0;
        gfx_SetColor(255);
        do
        {
            gfx_FillRectangle_NoClip(0, 10, 16, 230);
            gfx_PrintStringXY(">", 0, 10 * (selected + 1));
            while (!(key = os_GetCSC()))
                delay(10);
            switch (key)
            {
            case sk_Clear:
                gfx_End();
                return 0;
            case sk_Up:
                selected--;
                if (selected == 255)
                    selected = pack - 1;
                break;
            case sk_Down:
                selected++;
                if (selected == pack)
                    selected = 0;
                break;
            }
        } while (key != sk_Enter);
        keywords = os_GetAppVarData(packs[selected], NULL);
    }
    kwds = (keywords_t *)keywords->data;

    bool contgame = true;
    while (contgame)
    {
        gfx_FillScreen(255);
        draw_Noose();
        lostlives = 0;
        letter = 0;
        word = randInt(0, kwds->length - 1);
        uint8_t wlen = kwds->words[word].length;
        char *wword = kwds->words[word].word;
        for (uint8_t i = 0; i < wlen; i++)
            revealed[i] = wword[i] < 'A' || wword[i] > 'Z';
        memset(missed, 0, sizeof(missed));
        memset(matched, 0, sizeof(missed));
        hints = (cheat >= CHEATED) ? 255 : hints;
        bool cont = true;
        while (cont)
        {
            draw_Hangman();
            draw_Word();
            draw_Missed();

            sk_key_t key;
            while (!(key = os_GetCSC()))
                delay(10);
            uint8_t givehint;
            switch (key)
            {
#pragma region cheatttt
            case sk_0:
                if (cheat == 0 || cheat == 2 || cheat == 3)
                    cheat++;
                else
                    cheat = 0;
                break;
            case sk_DecPnt:
                if (cheat == 1)
                    cheat++;
                else
                    cheat = 0;
                break;
            case sk_Vars:
                if (cheat == 4)
                    cheat++;
                else
                    cheat = 0;
                break;
#pragma endregion cheatttt
            case sk_Zoom:
                lostlives = LIVES + 1;
                draw_Hangman();
                break;
            case sk_Clear:
                cont = false;
                break;
            case sk_Del:
            case sk_Mode:
                cont = false;
                contgame = false;
                break;
            case sk_Yequ:
                if (hints)
                {
                    hints--;
                    draw_Hangman();
                    do
                    {
                        givehint = randInt(0, wlen - 1);
                    } while (revealed[givehint]);
                    entered(wword[givehint]);
                }
                break;
#pragma region letters
            case sk_Up:
                if (letter == 0 || letter == 1)
                    letter++;
                break;
            case sk_Down:
                if (letter == 2 || letter == 3)
                    letter++;
                break;
            case sk_Left:
                if (letter == 4 || letter == 6)
                    letter++;
                break;
            case sk_Right:
                if (letter == 5 || letter == 7)
                    letter++;
                break;
#pragma endregion letters
#pragma region more letters
            case sk_Math:
            case sk_Apps:
            case sk_Prgm:
            case sk_Recip:
            case sk_Sin:
            case sk_Cos:
            case sk_Tan:
            case sk_Power:
            case sk_Square:
            case sk_Comma:
            case sk_LParen:
            case sk_RParen:
            case sk_Div:
            case sk_Log:
            case sk_7:
            case sk_8:
            case sk_9:
            case sk_Mul:
            case sk_Ln:
            case sk_4:
            case sk_5:
            case sk_6:
            case sk_Sub:
            case sk_Store:
            case sk_1:
            case sk_2:
#pragma endregion more letters
                if (missed[chars[key] - 'A'] || matched[chars[key] - 'A'])
                    break;
                entered(chars[key]);
                break;
            }
            bool won = true;
            if (lostlives > LIVES)
            {
                iamdead();
                memset(revealed, 1, sizeof(revealed));
                draw_Word(224);
                draw_Missed();
                gfx_PrintStringXY("YOU LOST!", 120, 140);
                gfx_PrintStringXY("[press any key to restart]", 120, 150);
                while (!os_GetCSC())
                    delay(10);
                cont = false;
                won = false;
                hints = 0;
            }
            for (uint8_t i = 0; i < wlen; i++)
            {
                if (!revealed[i])
                {
                    won = false;
                    break;
                }
            }
            if (won)
            {
                iwon();
                hints++;
                draw_Word(7);
                gfx_PrintStringXY("YOU WON!", 120, 140);
                gfx_PrintStringXY("[press any key to restart]", 120, 150);
                while (!os_GetCSC())
                    delay(10);
                cont = false;
            }
        }
    }

    gfx_End();

    return 0;
}