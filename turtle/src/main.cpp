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
#include <TINYSTL/vector.h>

char buffer[512];

double xpos = LCD_WIDTH / 2, ypos = LCD_HEIGHT / 2;
int deg = 90;
bool pendown = true;

void waitkey()
{
    while (!os_GetCSC())
        delay(10);
}

tinystl::string substr(const tinystl::string &s, size_t pos, size_t len = SIZE_MAX)
{
    if (len > s.size() - pos)
        len = s.size() - pos;
    return tinystl::string(s.c_str() + pos, len);
}

tinystl::vector<tinystl::string> split(const tinystl::string &s, char c)
{
    tinystl::vector<tinystl::string> res;
    const tinystl::string *now = &s;
    tinystl::string ts;

    for (size_t i = 0; i < now->size(); i++)
    {
        if (now->c_str()[i] == c)
        {
            res.push_back(substr(*now, 0, i));
            ts = substr(*now, i + 1);
            now = &ts;
            i = 0;
        }
    }

    res.push_back(*now);
    return res;
}

void execute(const tinystl::string &str)
{
    tinystl::vector<tinystl::string> args = split(str, tSpace);
    if (args.empty())
        return;
    while (!args.empty())
    {
        char *cmd = (char *)malloc(args[0].size() + 1);
        strcpy(cmd, args[0].c_str());
        args.erase(args.begin());
        if (!strcasecmp(cmd, "FD"))
        {
            const int d = atoi(args[0].c_str());
            args.erase(args.begin());
            const double mx = d * cos(deg * M_PI / 180), my = -d * sin(deg * M_PI / 180);
            if (pendown)
                gfx_Line(xpos, ypos, xpos + mx, ypos + my);
            xpos += mx;
            ypos += my;
        }
        else if (!strcasecmp(cmd, "BK"))
        {
            const int d = atoi(args[0].c_str());
            args.erase(args.begin());
            const double mx = -d * cos(deg * M_PI / 180), my = d * sin(deg * M_PI / 180);
            if (pendown)
                gfx_Line(xpos, ypos, xpos + mx, ypos + my);
            xpos += mx;
            ypos += my;
        }
        else if (!strcasecmp(cmd, "LT"))
        {
            const int d = atoi(args[0].c_str());
            args.erase(args.begin());
            deg = (deg + d) % 360;
        }
        else if (!strcasecmp(cmd, "RT"))
        {
            const int d = atoi(args[0].c_str());
            args.erase(args.begin());
            deg -= d;
            while (deg < 0)
                deg += 360;
        }
        else if (!strcasecmp(cmd, "HOME"))
        {
            if (pendown)
                gfx_Line(xpos, ypos, LCD_WIDTH / 2, LCD_HEIGHT / 2);
            xpos = LCD_WIDTH / 2;
            ypos = LCD_HEIGHT / 2;
            deg = 0;
        }
        else if (!strcasecmp(cmd, "PU"))
        {
            pendown = false;
        }
        else if (!strcasecmp(cmd, "PD"))
        {
            pendown = true;
        }
        else if (!strcasecmp(cmd, "REPEAT"))
        {
            int times = atoi(args[0].c_str());
            args.erase(args.begin());
            tinystl::string s;
            do
            {
                s.append(args[0].c_str(), args[0].c_str() + args[0].size());
                const char space[] = {tSpace, 0};
                s.append(space, space + 1);
                args.erase(args.begin());
            } while (s.c_str()[s.size() - 2] != tRBrack);
            s = substr(s, 1, s.size() - 2);
            for (; times; times--)
                execute(s);
        }
        else if (!strcasecmp(cmd, "LABEL"))
        {
            tinystl::string s = args[0];
            args.erase(args.begin());
            const char *label = s.c_str();
            uint16_t size = s.size();
            if (label[0] == tString)
            {
                label++;
                size--;
            }
            for (uint16_t i = 0; i < size; i++)
            {
                gfx_RotateSprite(gfx_GetSpriteChar(label[i]), (gfx_sprite_t *)buffer, ((360 - deg) % 360) * 256 / 360);
                gfx_TransparentSprite((gfx_sprite_t *)buffer, xpos + 8 * i * cos(deg * M_PI / 180), ypos - 8 * i * sin(deg * M_PI / 180));
            }
        }
        free(cmd);
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

    tinystl::string code;
    uint16_t size = ti_GetSize(var);
    code.resize(size);
    ti_Read((void *)code.c_str(), size, 1, var);
    ti_Close(var);

    char *str = (char *)code.c_str();
    for (uint16_t i = 0; i < size; i++)
        if (str[i] == tEnter)
            str[i] = tSpace;

    gfx_Begin();
    gfx_SetMonospaceFont(8);
    gfx_SetTransparentColor(255);

    execute(code);

    while (!os_GetCSC())
        delay(10);
    gfx_End();

    return 0;
}
