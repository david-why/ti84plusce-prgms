#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <tice.h>
#include <graphx.h>
#include "book_bin.h"
#include "nummenu.hpp"

uint8_t buffer[8 * 16 + 2];
gfx_sprite_t *spr = (gfx_sprite_t *)buffer;

uint8_t fgcolors[] = {0, 255}, bgcolors[] = {255, 0};
constexpr uint8_t schemes = sizeof(fgcolors) / sizeof(uint8_t);
uint8_t scheme;

uint8_t height;

void draw(uint16_t index, uint16_t x, uint8_t y)
{
    for (uint8_t i = 0; i < height; i++)
    {
        uint8_t mask = get_symbol_row(index, i);
        for (uint8_t j = 0; j < 8; j++)
            spr->data[8 * i + j] = mask & (1 << (7 - j)) ? fgcolors[scheme] : bgcolors[scheme];
    }
    gfx_Sprite(spr, x, y);
}

uint8_t page, gt;

void updatescheme()
{
    gfx_SetTextFGColor(fgcolors[scheme]);
    gfx_SetTextBGColor(bgcolors[scheme]);
    gfx_SetTextTransparentColor(bgcolors[scheme]);
}

int main()
{
    os_ClrHome();
    uint32_t books = n_books();
    const char *name;
    if (!books)
    {
        while (!os_GetCSC())
            ;
        return 0;
    }
    if (books == 1)
    {
        name = NULL;
    }
    else
    {
        const char **names = get_names();
        gfx_Begin();
        gfx_SetDrawBuffer();
        int32_t sel = nummenu(names, books);
        gfx_End();
        if (sel < 0)
        {
            deinit_book();
            return 0;
        }
        name = names[sel];
    }
    if (!init_book(name))
    {
        while (!os_GetCSC())
            ;
        return 0;
    }

    gfx_Begin();
    gfx_SetDrawBuffer();

    height = get_height();
    uint8_t rows = get_rows(),
            cols = get_cols(),
            width = get_width(),
            yscl = height + get_space(),
            pages = get_pages();
    spr->width = width;
    spr->height = height;

    bool updated = true, part = false;
    while (true)
    {
        if (updated)
        {
            gfx_BlitScreen();
            if (!part)
            {
                gfx_PrintStringXY("Loading", 10, 231);
                gfx_SwapDraw();

                gfx_FillScreen(bgcolors[scheme]);
                for (uint8_t r = 0; r < rows; r++)
                {
                    for (uint16_t c = 0; c < cols; c++)
                    {
                        uint8_t index = get_char(page, r, c);
                        if (index)
                        {
                            if (index & 0b10000000) // full-width
                            {
                                uint16_t fwi = ((uint16_t)(index & 0b01111111) << 8) + get_char(page, r, c + 1);
                                draw(fwi - 1, c++ * width, r * yscl);
                                draw(fwi, c * width, r * yscl);
                            }
                            else
                            {
                                draw(index - 1, c * width, r * yscl);
                            }
                        }
                    }
                }
            }
            gfx_SetTextXY(140, 231);
            gfx_PrintUInt(page + 1, 1);
            gfx_PrintChar('/');
            gfx_PrintUInt(pages, 1);
            if (gt)
            {
                gfx_PrintStringXY("Goto: ", 200, 231);
                uint8_t color = gfx_SetColor(bgcolors[scheme]);
                gfx_FillRectangle_NoClip(gfx_GetTextX(), 231, 24, 8);
                gfx_SetColor(color);
                gfx_PrintUInt(gt, 1);
            }
            gfx_SwapDraw();
        }

        sk_key_t key;
        while (!(key = os_GetCSC()))
            ;
        if (key == sk_Clear)
            break;
        updated = true;
        part = false;
        switch (key)
        {
        case sk_Down:
            page++;
            if (page >= pages)
                page = 0;
            break;
        case sk_Up:
            if (page == 0)
                page = pages;
            page--;
            break;
        case sk_Left:
            if (scheme == 0)
                scheme = schemes;
            scheme--;
            updatescheme();
            break;
        case sk_Right:
            scheme++;
            if (scheme >= schemes)
                scheme = 0;
            updatescheme();
            break;
        case sk_0:
        case sk_1:
        case sk_2:
        case sk_3:
        case sk_4:
        case sk_5:
        case sk_6:
        case sk_7:
        case sk_8:
        case sk_9:
            gt = gt * 10 + key2int(key);
            part = true;
            break;
        case sk_Del:
            gt /= 10;
            part = true;
            break;
        case sk_Enter:
            if (gt && gt <= pages)
            {
                page = gt - 1;
                updated = true;
            }
            gt = 0;
            break;
        default:
            updated = false;
            break;
        }
    }

    gfx_End();
    deinit_book();
    return 0;
}
