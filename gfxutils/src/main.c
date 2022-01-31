#ifndef __INT24_TYPE__
#define __INT24_TYPE__ int
#define __INT24_MAX__ 0x7fffff
#define __INT24_MIN__ (~__INT24_MAX__)
typedef __INT24_TYPE__ int24_t;
#define __UINT24_TYPE__ unsigned int
#define __UINT24_MAX__ 0xffffff
typedef __UINT24_TYPE__ uint24_t;
#endif

#include <tice.h>
#include <stdio.h>

#include "gfxutils.h"
#include "gfx/gfx.h"

char buf[256];

extern const gfx_sprite_t *bgsprite;

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(palette, sizeof_palette, 0);
    gfxutils_SetForegroundColor(gfxutils_FindClosestColor(255, 255, 255));
    gfxutils_SetBackgroundColor(gfxutils_FindClosestColor(0, 0, 0));
    gfxutils_SetBackgroundSprite(image);
    gfxutils_ClearScreen();
    os_GetKey();
    gfxutils_GetTextInput("TEST", buf, 10);
    gfx_End();
    os_ClrHome();
    os_PutStrFull(buf);
    os_GetKey();

    return 0;
}
