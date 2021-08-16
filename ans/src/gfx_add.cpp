#ifndef _GFX_ADD_H
#define _GFX_ADD_H

#include <graphx.h>

void gfx_ScaledTextXY(const char *text, int x, int y, uint8_t scale, gfx_sprite_t *buf = 0)
{
    unsigned int size = 8 * scale;
    if (buf == 0)
        buf = gfx_MallocSprite(size, size);
    unsigned int i = x;
    for (const char *c = text; *c != 0; c++)
    {
        gfx_ScaleSprite(gfx_GetSpriteChar(*c), buf);
        gfx_TransparentSprite(buf, i, y);
        i += size;
    }
}

#endif /* _GFX_ADD_H */
