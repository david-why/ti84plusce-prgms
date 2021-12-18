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
#include <graphx.h>
#include <fileioc.h>
#include <string.h>
#include <keypadc.h>

#include "util.hpp"
#include "nummenu.hpp"
#include "exec.hpp"
#include "games.hpp"

char buf[9];
uint8_t sel2index[NGAMES];
const char *names[NGAMES + 1];

int main()
{
    os_ClrHome();
    settings *s = get_settings();
    if (s->reqpwd)
    {
        enterpwd((sk_key_t *)buf, s->pwdlen);
        for (uint8_t i = 0; i < s->pwdlen; i++)
        {
            if (s->password[i] != buf[i])
            {
                save_settings(s);
                return 0;
            }
        }
    }
    s->reqpwd = s->protect;
    save_settings(s);

    names[1] = games[0].name;
    sel2index[1] = 0;
    uint8_t found = 1;
    void *search_pos = NULL;
    char *name;
    while ((name = ti_DetectVar(&search_pos, NULL, TI_PPRGM_TYPE)))
    {
        strcpy(buf, name);
        if (!(buf[0] & 64))
            buf[0] |= 64;
        for (uint8_t i = 1; i < NGAMES; i++)
        {
            if (!strcasecmp(buf, games[i].name))
            {
                names[++found] = games[i].name;
                sel2index[found] = i;
                break;
            }
        }
    }
    while ((name = ti_DetectVar(&search_pos, NULL, TI_PRGM_TYPE)))
    {
        strcpy(buf, name);
        if (!(buf[0] & 64))
            buf[0] |= 64;
        for (uint8_t i = 1; i < NGAMES; i++)
        {
            if (!strcasecmp(buf, games[i].name))
            {
                names[++found] = games[i].name;
                sel2index[found] = i;
                break;
            }
        }
    }

    if (!found)
    {
        print("No games found!");
        while (!os_GetCSC())
            delay(10);
        return 0;
    }

    names[0] = "----- Settings -----";

    os_GetCSC();
    gfx_Begin();
    gfx_SetDrawBuffer();
    while (true)
    {
        int16_t choice = s->prev_sel ? s->prev_sel : nummenu(names, found + 1, "The Game Console: Select Game");
        if (choice < 0)
            break;
        if (choice == 0)
        {
            s = get_settings();
            while (true)
            {
                int16_t choice = nummenu((const char *[]){"Turn on/off password protection", "Set new password"}, 2, "Settings");
                if (choice == 0)
                    s->protect = s->reqpwd = gfxutil_YesNo("Password protection?");
                else if (choice == 1)
                {
                    gfx_End();
                    os_ClrHome();
                    s->pwdlen = enterpwd((sk_key_t *)s->password, 6);
                    gfx_Begin();
                    gfx_SetDrawBuffer();
                }
                else
                    break;
            }
            save_settings(s);
        }
        else
        {
            s = get_settings();
            s->prev_sel = choice;
            save_settings(s);
            while (games[sel2index[choice]].action())
                ;
            s = get_settings();
            s->prev_sel = 0;
            save_settings(s);
        }
    }

    gfx_End();
    return 0;
}
