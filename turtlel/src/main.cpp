#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef signed int int24_t;
#endif

#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <TINYSTL/string.h>

char buffer[512];

double xpos = LCD_WIDTH / 2, ypos = LCD_HEIGHT / 2;
int deg;
bool pendown = true;

void waitkey()
{
    while (!os_GetCSC())
        delay(10);
}

tinystl::string getline(ti_var_t var)
{
    uint16_t size = ti_GetSize(var);
    uint16_t now = ti_Tell(var);
    tinystl::string s;
    while (now < size)
    {
        char c;
        ti_Read(&c, 1, 1, var);
        now++;
        if (c == tEnter)
            break;
        s.append(&c, &c + 1);
    }
    return s;
}

tinystl::string substr(const tinystl::string &s, size_t begin, size_t len = SIZE_MAX)
{
    if (begin + len > s.size())
        len = s.size() - begin;
    return tinystl::string(s.c_str() + begin, len);
}

struct split_result
{
    tinystl::string a, b;
};

split_result split(const tinystl::string &s, char c)
{
    const size_t size = s.size();
    for (size_t i = 0; i < size; i++)
        if (s.c_str()[i] == c)
            return {substr(s, 0, i), substr(s, i + 1, s.size() - i - 1)};
    return {s, tinystl::string("", 0)};
}

split_result rsplit(const tinystl::string &s, char c)
{
    const size_t size = s.size();
    for (size_t i = size; i > 0; i--)
        if (s.c_str()[i - 1] == c)
            return {substr(s, 0, i - 1), substr(s, i, s.size() - i)};
    return {s, tinystl::string("", 0)};
}

void run(const tinystl::string &s)
{
    split_result r = split(s, tSpace);
    const char *cmd = r.a.c_str(), *op = r.b.c_str();
    if (!strcasecmp(cmd, "FD") || !strcasecmp(cmd, "FORWARD"))
    {
        int c = atoi(op);
        double mx = c * cos(deg * M_PI / 180), my = c * sin(deg * M_PI / 180);
        if (pendown)
            gfx_Line(xpos, ypos, xpos + mx, ypos - my);
        xpos += mx;
        ypos -= my;
    }
    else if (!strcasecmp(cmd, "BK") || !strcasecmp(cmd, "BACK"))
    {
        int c = atoi(op);
        double mx = c * cos(deg * M_PI / 180), my = c * sin(deg * M_PI / 180);
        if (pendown)
            gfx_Line(xpos, ypos, xpos - mx, ypos + my);
        xpos -= mx;
        ypos += my;
    }
    else if (!strcasecmp(cmd, "LT") || !strcasecmp(cmd, "LEFT"))
    {
        int c = atoi(op) % 360;
        deg = (deg + c) % 360;
    }
    else if (!strcasecmp(cmd, "RT") || !strcasecmp(cmd, "RIGHT"))
    {
        int c = atoi(op) % 360;
        deg -= c;
        while (deg < 0)
            deg += 360;
    }
    else if (!strcasecmp(cmd, "HOME"))
    {
        xpos = LCD_WIDTH / 2;
        ypos = LCD_HEIGHT / 2;
        deg = 0;
    }
    else if (!strcasecmp(cmd, "CLEAN"))
    {
        gfx_FillScreen(255);
    }
    else if (!strcasecmp(cmd, "CS") || !strcasecmp(cmd, "CLEARSCREEN"))
    {
        gfx_FillScreen(255);
        xpos = LCD_WIDTH / 2;
        ypos = LCD_HEIGHT / 2;
        deg = 0;
    }
    else if (!strcasecmp(cmd, "PU") || !strcasecmp(cmd, "PENUP"))
    {
        pendown = false;
    }
    else if (!strcasecmp(cmd, "PD") || !strcasecmp(cmd, "PENDOWN"))
    {
        pendown = true;
    }
    else if (!strcasecmp(cmd, "LABEL"))
    {
        if (r.b.size() != 0)
        {
            size_t i = 0;
            size_t size = r.b.size();
            if (op[0] == tString)
            {
                op++;
                size--;
            }
            for (; i < r.b.size(); i++)
            {
                gfx_RotateSprite(gfx_GetSpriteChar(op[i]), (gfx_sprite_t *)buffer, ((360 - deg) % 360) * 256 / 360);
                gfx_TransparentSprite((gfx_sprite_t *)buffer, xpos + 8 * i * cos(deg * M_PI / 180), ypos - 8 * i * sin(deg * M_PI / 180));
            }
        }
    }
}

int main()
{
    os_ClrHome();
    os_GetStringInput((char *)"Please enter program name\xD6", buffer, 9);
    os_SetCursorPos(2, 0);

    void *search_pos = NULL;
    char *name;
    bool found = false;
    while ((name = ti_DetectVar(&search_pos, NULL, TI_PRGM_TYPE)))
    {
        if (!strcmp(name, buffer))
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        os_PutStrFull("Program not found!");
        waitkey();
        return 0;
    }

    ti_CloseAll();
    ti_var_t var = ti_OpenVar(name, "r", TI_PRGM_TYPE);
    if (var == 0)
    {
        os_PutStrFull("ERROR: Open failed!");
        waitkey();
        return 0;
    }

    gfx_Begin();
    gfx_SetMonospaceFont(8);
    gfx_SetTransparentColor(255);

    tinystl::string s;
    while ((s = getline(var)).size() != 0)
    {
        run(s);
    }

    waitkey();
    gfx_End();

    return 0;
}
