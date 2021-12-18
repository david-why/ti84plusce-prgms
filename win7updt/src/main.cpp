#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <keypadc.h>
#include <tice.h>
#include <graphx.h>
#include "gfx/gfx.h"

int main()
{
    gfx_Begin();
    gfx_Sprite_NoClip(update_calc_l, 0, 0);
    gfx_Sprite_NoClip(update_calc_r, 160, 0);
    gfx_SetPalette(palette, sizeof_palette, myimages_palette_offset);
    gfx_SetTextTransparentColor(254);
    gfx_SetTextBGColor(254);
    gfx_SetTextFGColor(255);
    gfx_PrintStringXY("Configuring Windows Updates", 90, 92);
    gfx_PrintStringXY("2% complete.", 135, 107);
    gfx_PrintStringXY("Do not turn off your computer.", 85, 122);

    do
    {
        kb_Scan();
        delay(10);
    } while (!(kb_IsDown(kb_Key6) && kb_IsDown(kb_KeySquare) && kb_IsDown(kb_KeyDiv))); // Key: VIM
    // } while (!(kb_IsDown(kb_KeyClear)));

    gfx_End();
    return 0;
}
