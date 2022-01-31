/**
 * GfxUtils - utils to use in conjuction with the GRAPHX library
 * @author david-why
 */

#ifndef _GFXUTILS_H
#define _GFXUTILS_H

#include <graphx.h>

/**
 * Displays a box in the middle of the screen to get text input from the user. 
 * Uses the background color set via \c gfxutils_SetBackgroundColor().
 * 
 * @param title The title to display in the box (6 letters or less preferred).
 * @param buffer The buffer for the user to type in data.
 * @param length Maximum text length for the user to input. Does not include
 * string terminator.
 * @returns Number of characters the user typed in.
 */
uint16_t gfxutils_GetTextInput(const char *title, char *buffer, uint16_t length);

/**
 * Sets the global background color used in various gfxutils functions. 
 * This also calls \c gfx_SetTextBGColor() and 
 * \c gfx_SetTextTransparentcolor().
 * 
 * @param color The background color to set.
 * @returns The previous text background color.
 */
uint8_t gfxutils_SetBackgroundColor(uint8_t color);

/**
 * Sets the global foreground color used in various gfxutils functions. 
 * This also calls \c gfx_SetTextFGColor().
 * 
 * @param color The foreground color to set.
 * @returns The previous text foreground color.
 */
uint8_t gfxutils_SetForegroundColor(uint8_t color);

/**
 * Sets the background image. 
 * This image will display when you call \c gfxutils_ClearScreen().
 * 
 * @param sprite The background image to set, or \c NULL to disable background
 * image. Image will be scaled to fit.
 * @returns The previously set background image.
 * @see gfxutils_ClearScreen
 */
const gfx_sprite_t *gfxutils_SetBackgroundSprite(const gfx_sprite_t *sprite);

/**
 * Displays an image in fullscreen, scaling as required.
 * 
 * @param sprite The image to draw.
 * @returns The scale used.
 */
uint8_t gfxutils_FullScreenSprite(const gfx_sprite_t *sprite);

/**
 * Clears the screen. If a background image is set, display the image. Else 
 * fill the screen with background color. Will blit buffer.
 * 
 * @see gfxutils_SetBackgroundSprite
 * @see gfxutils_SetBackgroundColor
 */
void gfxutils_ClearScreen(void);

/**
 * Structure of a 24-bit RGB color, with 8 bits of red, green, and blue.
 */
typedef struct
{
    uint8_t r, g, b;
} gfxutils_rgb888_t;

/**
 * Finds the closest RGB color in the current palette.
 * 
 * @param r The red value.
 * @param g The green value.
 * @param b The blue value.
 * @returns The closest color index in the current palette.
 */
uint8_t gfxutils_FindClosestColor(uint8_t r, uint8_t g, uint8_t b);

/**
 * Convert a 16-bit RGB565 color to a RGB888 color.
 * 
 * @param rgb565 The RGB565 color, typically found in the GRAPHX palette.
 * @returns The RGB888 color.
 */
gfxutils_rgb888_t gfxutils_565To888(uint16_t rgb565);

#endif