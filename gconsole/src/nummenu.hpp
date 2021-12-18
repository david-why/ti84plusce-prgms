#ifndef _NUMMENU_H
#define _NUMMENU_H

#include <tice.h>
#include <graphx.h>
#include "TINYSTL/vector.h"

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

int16_t nummenu(const char *names[], uint8_t len, const char *title = NULL)
{
    uint8_t num = 0, page = 0, pages = (len + 9) / 10;
    while (true)
    {
        gfx_FillScreen(255);
        if (title != NULL)
            gfx_PrintStringXY(title, 10, 5);
        for (uint8_t i = page * 10; i < len && i < page * 10 + 10; i++)
        {
            gfx_SetTextXY(10, (i % 10) * 15 + 20);
            gfx_PrintUInt(i + 1, 3);
            gfx_PrintString(" ");
            gfx_PrintString(names[i]);
        }
        gfx_PrintStringXY("Choice: ", 20, 180);
        gfx_PrintUInt(num, 3);
        gfx_PrintString("?");
        if (pages > 1)
        {
            gfx_PrintStringXY("Page ", 20, 200);
            gfx_PrintUInt(page + 1, 1);
            gfx_PrintString("/");
            gfx_PrintUInt(pages, 1);
        }
        gfx_SwapDraw();

        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Clear)
            return -1;
        if (key == sk_Enter)
        {
            if (num <= len && num)
                return num - 1;
            num = 0;
        }
        switch (key)
        {
        case sk_Right:
        case sk_Down:
            if (page * 10 + 10 < len)
                page++;
            break;
        case sk_Left:
        case sk_Up:
            if (page)
                page--;
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
            num = num * 10 + key2int(key);
            break;
        case sk_Del:
            num /= 10;
            break;
        }
    }
}

template<typename T>
int16_t nummenu(const char *(*func)(T &), tinystl::vector<T> vec, uint8_t len, const char *title = NULL)
{
    uint8_t num = 0, page = 0, pages = (len + 9) / 10;
    while (true)
    {
        gfx_FillScreen(255);
        if (title != NULL)
            gfx_PrintStringXY(title, 10, 5);
        for (uint8_t i = page * 10; i < len && i < page * 10 + 10; i++)
        {
            gfx_SetTextXY(10, (i % 10) * 15 + 20);
            gfx_PrintUInt(i + 1, 3);
            gfx_PrintString(" ");
            gfx_PrintString(func(vec[i]));
        }
        gfx_PrintStringXY("Choice: ", 20, 180);
        gfx_PrintUInt(num, 3);
        gfx_PrintString("?");
        if (pages > 1)
        {
            gfx_PrintStringXY("Page ", 20, 200);
            gfx_PrintUInt(page + 1, 1);
            gfx_PrintString("/");
            gfx_PrintUInt(pages, 1);
        }
        gfx_SwapDraw();

        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Clear)
            return -1;
        if (key == sk_Enter)
        {
            if (num <= len && num)
                return num - 1;
            num = 0;
        }
        switch (key)
        {
        case sk_Right:
        case sk_Down:
            if (page * 10 + 10 < len)
                page++;
            break;
        case sk_Left:
        case sk_Up:
            if (page)
                page--;
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
            num = num * 10 + key2int(key);
            break;
        case sk_Del:
            num /= 10;
            break;
        }
    }
}

#endif /* _NUMMENU_H */