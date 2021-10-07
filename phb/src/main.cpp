#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <tice.h>
#include <graphx.h>
#include <stdio.h>
#include <string.h>

#include "pages.hpp"

constexpr uint8_t key2int(sk_key_t key)
{
    if (key == sk_1)
        return 1;
    if (key == sk_2)
        return 2;
    if (key == sk_3)
        return 3;
    if (key == sk_4)
        return 4;
    if (key == sk_5)
        return 5;
    if (key == sk_6)
        return 6;
    if (key == sk_7)
        return 7;
    if (key == sk_8)
        return 8;
    if (key == sk_9)
        return 9;
    return 0;
}

void disppage(unsigned int chosen_page)
{
    const page &p = pages[chosen_page];
    gfx_FillScreen(255);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY(p.title, (LCD_WIDTH - gfx_GetStringWidth(p.title)) / 2, 10);
    gfx_SetTextScale(1, 1);
    gfx_SetTextXY(10, 35);
    const size_t length = strlen(p.content);
    for (size_t i = 0; i < length; i++)
    {
        char c = p.content[i];
        switch (c)
        {
        case 1:
            gfx_SetTextFGColor(224);
            break;
        case 2:
            gfx_SetTextFGColor(0);
            break;
        case '\n':
            gfx_SetTextXY(10, gfx_GetTextY() + 10);
            break;
        default:
            gfx_PrintChar(c);
            break;
        }
    }
    gfx_SwapDraw();
}

char buffer[256];

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTransparentColor(255);

    unsigned int on_page = 0;
    sk_key_t key;
    while (true)
    {
        gfx_FillScreen(255);
        gfx_SetTextScale(2, 2);
        const char *title = "The Portable Book";
        gfx_PrintStringXY(title, (LCD_WIDTH - gfx_GetStringWidth(title)) / 2, 10);
        gfx_SetTextScale(1, 1);
        gfx_PrintChar('P');
        gfx_PrintUInt(on_page + 1, 1);

        unsigned int maxshow = (on_page * 8 + 8);
        if (maxshow >= n_pages)
            maxshow = n_pages;
        for (unsigned int i = on_page * 8; i < maxshow; i++)
        {
            if (strlen(pages[i].title) > 0)
            {
                sprintf(buffer, "[%u] %s", i % 8, pages[i].title);
                gfx_PrintStringXY(buffer, 20, (i % 8) * 25 + 35);
            }
        }
        gfx_SwapDraw();

        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Clear)
            break;
        unsigned int chosen_page = -1;
        switch (key)
        {
        case sk_Left:
            if (on_page == 0)
                on_page = n_bigpages;
            on_page--;
            break;
        case sk_Right:
            on_page++;
            if (on_page == n_bigpages)
                on_page = 0;
            break;
        case sk_0:
        case sk_1:
        case sk_2:
        case sk_3:
        case sk_4:
        case sk_5:
        case sk_6:
        case sk_7:
            chosen_page = on_page * 8 + key2int(key);
            break;
        }

        if (chosen_page == (unsigned int)(-1) || chosen_page >= n_pages)
            continue;

        disppage(chosen_page);

        while (true)
        {
            while (!(key = os_GetCSC()))
                delay(10);
            if (key == sk_Right)
            {
                chosen_page++;
                if (chosen_page == n_pages)
                    chosen_page = 0;
                disppage(chosen_page);
            }
            else if (key == sk_Left)
            {
                if (chosen_page == 0)
                    chosen_page = n_pages;
                chosen_page--;
                disppage(chosen_page);
            }
            else
            {
                break;
            }
        }
    }

    gfx_End();
    return 0;
}
