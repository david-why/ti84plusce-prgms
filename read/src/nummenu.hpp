#ifndef _NUMMENU_H
#define _NUMMENU_H

#include <tice.h>
#include <graphx.h>

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

int32_t nummenu(const char *names[], uint16_t len)
{
    uint16_t num = 0, page = 0;
    while (true)
    {
        gfx_FillScreen(255);
        for (uint16_t i = page * 10 + 1; i <= len && i <= page * 10 + 10; i++)
        {
            gfx_SetTextXY(10, ((i - 1) % 10) * 15 + 10);
            gfx_PrintUInt(i, 3);
            gfx_PrintString(" ");
            gfx_PrintString(names[i - 1]);
        }
        gfx_PrintStringXY("Choice: ", 20, 170);
        gfx_PrintUInt(num, 3);
        gfx_PrintString("?");
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