#include <graphx.h>

#ifndef uint24_t
typedef unsigned int uint24_t;
#endif /* uint24_t */

typedef struct pos_t
{
    int x;
    int16_t y;

    pos_t(int, uint16_t) {}
    pos_t() {}
} pos_t;

pos_t gfx_CenteredScaledSprite(const gfx_sprite_t *sprite, gfx_sprite_t *buff, uint8_t width_scale, uint8_t height_scale);
pos_t gfx_CenteredScaledSprite(const gfx_sprite_t *sprite, uint8_t width_scale, uint8_t height_scale);
void gfx_PrintScaledStringXY(const char *string, int x, int y, double scale, uint16_t color = 256, uint16_t keep = 256);
void gfx_PrintCenteredString(const char *string, int y);
#define gfx_PrintCenteredScaledString(string, y, scale, color, bgcolor) \
    gfx_PrintScaledStringXY((string), (LCD_WIDTH - gfx_GetStringWidth(string) * (scale)) / 2, (y), (scale), (color), (bgcolor))
gfx_sprite_t *gfx_ScaleSprite(const gfx_sprite_t *sprite_in, gfx_sprite_t *sprite_out, uint8_t width_scale, uint8_t height_scale);
void gfx_ScaledSprite(const gfx_sprite_t *sprite, uint24_t x, uint8_t y, uint8_t width_scale, uint8_t height_scale);
void gfx_ColorSprite(gfx_sprite_t *sprite, uint8_t color, uint16_t keep = 256);
