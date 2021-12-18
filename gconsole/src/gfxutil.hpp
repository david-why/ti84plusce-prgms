#ifndef _GFXUTIL_HPP
#define _GFXUTIL_HPP

#include <tice.h>
#include <graphx.h>

void gfxutil_Error(const char *str)
{
    gfx_FillScreen(255);
    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(224);
    gfx_PrintStringXY(str, 10, (LCD_HEIGHT - 8 * 2) / 2);
    gfx_SwapDraw();
    while (!os_GetCSC())
        delay(10);
    gfx_SetTextFGColor(0);
    gfx_SetTextScale(1, 1);
    return;
}
void gfxutil_Info(const char *str)
{
    gfx_FillScreen(255);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY(str, 10, (LCD_HEIGHT - 8 * 2) / 2);
    gfx_SwapDraw();
    while (!os_GetCSC())
        delay(10);
    gfx_SetTextScale(1, 1);
    return;
}
bool gfxutil_YesNo(const char *str)
{
    gfx_FillScreen(255);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY(str, 10, 70);
    gfx_SetTextScale(1, 1);
    gfx_HorizLine(0, 225, 50);
    gfx_VertLine(50, 225, 15);
    gfx_PrintStringXY("Yes", 13, 230);
    gfx_HorizLine(270, 225, 50);
    gfx_VertLine(270, 225, 15);
    gfx_PrintStringXY("No", 288, 230);
    gfx_SwapDraw();
    while (true)
    {
        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        switch (key)
        {
        case sk_Yequ:
            return true;
        case sk_Graph:
            return false;
        }
    }
}

#endif /* _GFXUTIL_HPP */