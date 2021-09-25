#include <tice.h>
#include <graphx.h>
#include <stdio.h>
#include <math.h>
#include <keypadc.h>

uint8_t mode;
const uint8_t maxmode = 2;
char buffer[10];

void show(uint8_t h, uint8_t m, uint8_t s)
{
    sprintf(buffer, "%02u:%02u:%02u", h, m, s);
    gfx_PrintStringXY(buffer, (LCD_WIDTH - gfx_GetStringWidth(buffer)) / 2, 100);
}

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(224);

    while (true)
    {
        gfx_FillScreen(255);
        uint8_t h = rtc_Hours, m = rtc_Minutes, s = rtc_Seconds + 1;
        if (s == 60)
        {
            s = 0;
            m++;
            if (m == 60)
            {
                m = 0;
                h++;
                if (h == 24)
                    h = 0;
            }
        }
        if (mode == 0)
        {
            gfx_SetTextFGColor(0);
            gfx_SetTextScale(2, 2);
            gfx_SetTextXY(10, 10);
            gfx_PrintUInt(h, 2);
            gfx_PrintChar(':');
            gfx_PrintUInt(m, 2);
            gfx_PrintChar(':');
            gfx_PrintUInt(s, 2);

            gfx_Circle(160, 120, 100);
            gfx_Circle(160, 120, 101);
            gfx_FillCircle_NoClip(160, 120, 2);
            for (uint8_t i = 0; i < 12; i++)
                gfx_Line_NoClip(100 * cos(i * M_PI / 6) + 160, 100 * sin(i * M_PI / 6) + 120, 95 * cos(i * M_PI / 6) + 160, 95 * sin(i * M_PI / 6) + 120);
            gfx_Line_NoClip(160, 120, 160 + 50 * cos(h * M_PI / 6 - M_PI_2 + m * M_PI / 360) + 0.5, 120 + 50 * sin(h * M_PI / 6 - M_PI_2 + m * M_PI / 360) + 0.5);
            gfx_Line_NoClip(160, 120, 160 + 75 * cos(m * M_PI / 30 - M_PI_2 + s * M_PI / 360 / 5) + 0.5, 120 + 75 * sin(m * M_PI / 30 - M_PI_2 + s * M_PI / 360 / 5) + 0.5);
            gfx_Line_NoClip(160, 120, 160 + 90 * cos(s * M_PI / 30 - M_PI_2) + 0.5, 120 + 90 * sin(s * M_PI / 30 - M_PI_2) + 0.5);
        }
        else if (mode == 1)
        {
            gfx_SetTextScale(5, 5);
            show(h, m, s);
        }
        else if (mode == 2)
        {
            gfx_SetTextFGColor(randInt(0, 254));
            show(h, m, s);
        }
        delay(500);
        while (rtc_Seconds != s && !kb_IsDown(kb_KeyClear) && !kb_IsDown(kb_KeyMode))
            kb_Scan();

        // sk_key_t key = os_GetCSC();
        // if (key == sk_Clear)
        //     break;
        // if (key == sk_Mode)
        // {
        //     mode++;
        //     if (mode > maxmode)
        //         mode = 0;
        // }
        if (kb_IsDown(kb_KeyClear))
        {
            do
                kb_Scan();
            while (kb_IsDown(kb_KeyClear));
            break;
        }
        if (kb_IsDown(kb_KeyMode))
        {
            do
                kb_Scan();
            while (kb_IsDown(kb_KeyMode));
            mode++;
            if (mode > maxmode)
                mode = 0;
        }
        else
        {
            gfx_SwapDraw();
        }
    }

    gfx_End();
    return 0;
}
