#include <fontlibc.h>

static const uint8_t font_lucida_console_data[] = {
    #include "LucidaConsole.inc"
};
const fontlib_font_t *font_lucida_console = (fontlib_font_t *)font_lucida_console_data;
