#include <graphx.h>
#include <tice.h>
#include <fileioc.h>

#include <stdio.h>
#include <string.h>

#include "gfx/gfx.h"

#define WHITE 0
#define BLACK 1
#define RED 2
#define GREEN 3
#define BLUE 4

#define XMIN (SCALE / 2)
#define XMAX (LCD_WIDTH - XMIN)
#define YMIN (SCALE / 2)
#define YMAX (LCD_HEIGHT - YMIN)

#define abs(x) ((x) < 0 ? (-(x)) : (x))

#ifndef uint24_t
typedef unsigned int uint24_t;
#endif /* uint24_t */

#ifndef UINT24_MAX
#define UINT24_MAX UINT32_MAX
#endif /* UINT24_MAX */

#ifndef INT24_MAX
#define INT24_MAX INT32_MAX
#endif /* UINT24_MAX */

typedef struct pos_t
{
    uint24_t x;
    uint16_t y;
    bool working;

    pos_t() : x(0), y(0), working(0) {}
    pos_t(uint24_t x, uint16_t y) : x(x), y(y), working(1) {}
    pos_t(uint24_t x, uint16_t y, bool working) : x(x), y(y), working(working) {}
} pos_t;

typedef struct score_t
{
    unsigned int score;
    char name[15];

    score_t() : score(0), name("ANONYMOUS") {}
    score_t(unsigned int score) : score(score), name("ANONYMOUS") {}
    score_t(unsigned int score, const char *n) : score(score)
    {
        strcpy(name, n);
    }
} score_t;

typedef struct settings_t
{
    uint8_t SCALE;
    uint8_t FIRSTWAVE;
    uint8_t LEVELUPADD;
} settings_t;

uint8_t SCALE = 5;
uint8_t FIRSTWAVE = 10;
uint8_t LEVELUPADD = 2;
unsigned int score;
score_t highscore = UINT24_MAX;
pos_t current;
pos_t robots[100];
uint8_t robots_len;
bool dead, win = true, exited;
char out[25];

void gfx_PrintCenteredScaledString(const char *string,
                                   int y,
                                   double scale)
{
    const char *ptr;
    uint24_t x = (LCD_WIDTH - gfx_GetStringWidth(string) * scale) / 2;
    uint16_t scaled = scale * 8;
    gfx_sprite_t *chr = gfx_MallocSprite(scaled, scaled);

    for (ptr = string; *ptr != '\0'; ptr++)
    {
        gfx_ScaleSprite(gfx_GetSpriteChar(*ptr), (gfx_sprite_t *)chr);
        gfx_Sprite((gfx_sprite_t *)chr, x, y);
        x += scaled;
    }
}

void drawPixel(int x, int y)
{
    gfx_FillRectangle(x, y, SCALE, SCALE);
}

void drawPixelColor(uint24_t x, uint16_t y, uint8_t color)
{
    uint8_t before = gfx_SetColor(color);
    drawPixel(x, y);
    gfx_SetColor(before);
}

void updateScreen()
{
    gfx_FillScreen(WHITE);
    gfx_SetColor(BLUE);
    gfx_Rectangle(XMIN - 1, YMIN - 1, XMAX - XMIN + 2, YMAX - YMIN + 2);
    drawPixel(current.x, current.y);
    gfx_SetColor(RED);
    uint8_t i;
    for (i = 0; i < robots_len; i++)
    {
        if (robots[i].working)
        {
            drawPixel(robots[i].x, robots[i].y);
        }
        else
        {
            drawPixelColor(robots[i].x, robots[i].y, BLACK);
        }
    }
    gfx_BlitBuffer();
}

void updateRobots()
{
    uint8_t i, j;
    win = true;
    for (i = 0; i < robots_len; i++)
    {
        if (robots[i].working)
        {

            win = false;
            if (robots[i].x > current.x)
            {
                robots[i].x -= SCALE;
            }
            else if (robots[i].x < current.x)
            {
                robots[i].x += SCALE;
            }
            if (robots[i].y > current.y)
            {
                robots[i].y -= SCALE;
            }
            else if (robots[i].y < current.y)
            {
                robots[i].y += SCALE;
            }
        }
        if (robots[i].x == current.x && robots[i].y == current.y)
        {
            dead = true;
        }
        for (j = 0; j < robots_len; j++)
        {
            if ((j < i || !robots[j].working) && robots[i].x == robots[j].x && robots[i].y == robots[j].y)
            {
                if (robots[i].working)
                {
                    score++;
                }
                if (robots[j].working)
                {
                    score++;
                }
                robots[i].working = false;
                robots[j].working = false;
                break;
            }
        }
    }
}

void updateHighscore()
{
    var_t *var = os_GetAppVarData("ROBOTShs", NULL);
    if (var == NULL)
    {
        score_t score_save;
        var = os_CreateAppVar("ROBOTShs", sizeof(score_t));
        if (var == NULL)
        {
            return;
        }
        memcpy(var->data, &score_save, sizeof(score_t));
    }
    highscore = *(score_t *)var->data;
}

unsigned int currentHighscore()
{
    if (highscore.score == UINT24_MAX)
    {
        updateHighscore();
    }
    return highscore.score > score ? highscore.score : score;
}

bool setHighscore(bool return_to_gfx = true, bool force = false, bool interactive = true)
{
    updateHighscore();
    if (score < highscore.score && !force)
    {
        if (!return_to_gfx)
        {
            gfx_End();
        }
        return false;
    }
    if (interactive)
    {
        gfx_End();
        os_ClrHome();
        os_PutStrFull("Please enter your name (14 chars):");
        outchar('\n');
        os_GetStringInput("", out, 15);
        if (strlen(out) > 1)
        {
            strcpy(highscore.name, out);
        }
        highscore.score = score;
    }
    var_t *var = os_GetAppVarData("ROBOTShs", NULL);
    if (!var)
    {
        return false;
    }
    memcpy(var->data, &highscore, sizeof(highscore));

    if (return_to_gfx)
    {
        gfx_Begin();
        gfx_SetDrawBuffer();
        gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    }

    return true;
}

bool IAmDead()
{
    uint16_t y;
    gfx_FillScreen(RED);
    for (y = 0; y < LCD_HEIGHT; y++)
    {
        gfx_BlitRectangle(gfx_buffer, 0, y, LCD_WIDTH, 1);
        if (os_GetCSC() == sk_Clear)
        {
            break;
        }
        boot_WaitShort();
    }
    // gfx_SetColor(WHITE);
    // gfx_FillRectangle_NoClip(LCD_WIDTH - 300, LCD_HEIGHT / 2 - 15, 600 - LCD_WIDTH, 40);
    gfx_FillScreen(WHITE);
    gfx_SetTextFGColor(BLACK);
    gfx_PrintCenteredScaledString("YOU ARE VERY DEAD.", LCD_HEIGHT / 2 - 10, 1.5);
    gfx_BlitBuffer();
    sprintf(out, "YOU SCORED: %d", score);
    gfx_PrintCenteredScaledString(out, LCD_HEIGHT / 2 + 10, 1.5);
    gfx_BlitBuffer();
    return !setHighscore();
}

bool IWin()
{
    uint16_t y;
    gfx_FillScreen(BLUE);
    for (y = 0; y < LCD_HEIGHT; y++)
    {
        gfx_BlitRectangle(gfx_buffer, 0, y, LCD_WIDTH, 1);
        if (os_GetCSC() == sk_Clear)
        {
            break;
        }
        boot_WaitShort();
    }
    gfx_FillScreen(WHITE);
    gfx_BlitBuffer();
    sprintf(out, "YOU WON ALL %d ROBOTS!", robots_len);
    gfx_PrintCenteredScaledString(out, LCD_HEIGHT / 2 - 10, 1.5);
    sprintf(out, "YOU SCORED: %d", score);
    gfx_PrintCenteredScaledString(out, LCD_HEIGHT / 2 + 10, 1.5);
    gfx_BlitBuffer();
    return !setHighscore();
}

void setRobots(uint8_t n_robots)
{
    uint8_t i;
    robots_len = n_robots;
    for (i = 0; i < n_robots; i++)
    {
        robots[i] = {(uint24_t)(XMIN + randInt(0, (XMAX - XMIN) / SCALE - 1) * SCALE),
                     (uint16_t)(YMIN + randInt(0, (YMAX - YMIN) / SCALE - 1) * SCALE)};
    }
    updateScreen();
}

bool move(int8_t x, int8_t y)
{
    uint24_t newx = current.x + x * SCALE;
    uint16_t newy = current.y + y * SCALE;
    if (newx >= XMIN && newx <= XMAX - SCALE)
    {
        current.x = newx;
    }
    if (newy >= YMIN && newy <= YMAX - SCALE)
    {
        current.y = newy;
    }

    updateRobots();
    updateScreen();
    if (dead)
    {
        return IAmDead();
    }
    if (win)
    {
        return IWin();
    }
    return true;
}

void teleport()
{
    current.x = (uint24_t)(XMIN + randInt(0, (XMAX - XMIN) / SCALE - 1) * SCALE);
    current.y = (uint16_t)(YMIN + randInt(0, (YMAX - YMIN) / SCALE - 1) * SCALE);
    updateRobots();
    updateScreen();
    if (dead)
    {
        uint8_t i;
        for (i = 0; i < 100; i++)
        {
            boot_WaitShort();
        }
        IAmDead();
    }
    if (win)
    {
        IWin();
    }
}

void showVars()
{
    gfx_FillScreen(WHITE);
    gfx_SetTextFGColor(BLUE);
    gfx_PrintCenteredScaledString("SCORE:", LCD_HEIGHT / 2 - 50, 2.5);
    sprintf(out, "%d", score);
    gfx_PrintCenteredScaledString(out, LCD_HEIGHT / 2 - 25, 1.5);
    gfx_SetTextFGColor(RED);
    gfx_PrintCenteredScaledString("HIGH:", LCD_HEIGHT / 2 - 10, 2.5);
    sprintf(out, "%d", currentHighscore());
    sprintf(out, "%s %s", out, highscore.name);
    gfx_PrintCenteredScaledString(out, LCD_HEIGHT / 2 + 15, 1.5);
    gfx_SetTextFGColor(BLACK);
    gfx_PrintCenteredScaledString("Press [Enter] to continue", LCD_HEIGHT / 2 + 50, 1);
    gfx_BlitBuffer();
    while (os_GetCSC() != sk_Enter)
        ;
    updateScreen();
}

int main()
{
    srandom(rtc_Time());
    os_ClrHome();

    settings_t settings;
    var_t *var = os_GetAppVarData("ROBOTSst", NULL);
    if (var != NULL)
    {
        memcpy(&settings, var->data, var->size);
    }
    else
    {
        settings = {5, 10, 2};
    }
    uint8_t type;
    os_GetAnsData(&type);
    if (type == TI_REAL_TYPE)
    {
        real_t *ans;
        uint24_t ans_i;
        ans = (real_t *)os_GetAnsData(&type);
        ans_i = os_RealToInt24(ans);
        if (ans_i > 2825000 && ans_i < 2825100)
        {
            settings.SCALE = ans_i % 100;
            sprintf(out, "Set SCALE=%d", settings.SCALE);
            os_PutStrFull(out);
            while (!os_GetCSC())
                ;
        }
        else if (ans_i > 2825100 && ans_i < 2825200)
        {
            settings.FIRSTWAVE = ans_i % 100;
            sprintf(out, "Set FIRSTWAVE=%d", settings.FIRSTWAVE);
            os_PutStrFull(out);
            while (!os_GetCSC())
                ;
        }
        else if (ans_i > 2825200 && ans_i < 2825300)
        {
            settings.LEVELUPADD = ans_i % 100;
            sprintf(out, "Set LEVELUPADD=%d", settings.LEVELUPADD);
            os_PutStrFull(out);
            while (!os_GetCSC())
                ;
        }
        else if (ans_i == 2825300)
        {
            settings = {5, 10, 2};
            updateHighscore();
            highscore = {0, "ANONYMOUS"};
            var_t *var = os_GetAppVarData("ROBOTShs", NULL);
            if (var)
            {
                memcpy(var->data, &highscore, sizeof(highscore));
            }
        }
    }
    else if (type == TI_STRING_TYPE)
    {
        string_t *ans = (string_t *)os_GetAnsData(&type);
        if (!strncasecmp(ans->data, "HELP", ans->len))
        {
            sk_key_t key;
            os_ClrHome();
            os_PutStrFull("This is THE robots game.  You want to dodge robots  that try to kill you and  destroy them.");
            outchar('\n');
            os_PutStrFull("Every time you destroy a  robot, you gain a point.  Your highscore will be    saved.");
            outchar('\n');
            outchar('\n');
            os_PutStrFull("(Any key:Cont,Clear:Quit)");
            while (!(key = os_GetCSC()))
                ;
            if (key == sk_Clear)
            {
                return 0;
            }
            outchar('\n');
            updateHighscore();
            os_PutStrFull("The current highscore is  held by");
            outchar('\n');
            os_PutStrFull(highscore.name);
            outchar('\n');
            os_PutStrFull("who holds the score of");
            outchar('\n');
            sprintf(out, "%d", highscore.score);
            os_PutStrFull(out);
            outchar('\n');
            os_PutStrFull("points.");
            outchar('\n');
            os_PutStrFull("Try your best to beat thatscore!");
            outchar('\n');
            os_PutStrFull("Created by David");
            outchar('\n');
            os_PutStrFull("(Any key:Quit)");
            while (!(key = os_GetCSC()))
                ;
            if (key == sk_2nd)
            {
                while (!(key = os_GetCSC()))
                    ;
                if (key != sk_Math)
                    return 0;
                outchar('\n');
                os_PutStrFull("Hey! How did you figure   THAT out??\?!");
                outchar('\n');
                outchar('\n');
                outchar('\n');
                os_PutStrFull("Anyways, happy playing!");
                outchar('\n');
                outchar('\n');
                outchar('\n');
                outchar('\n');
                outchar('\n');
                os_PutStrFull("(Any key:Really Quit)");
                while (!(key = os_GetCSC()))
                    ;
                if (key == sk_GraphVar)
                {
                    os_ClrHome();
                    os_PutStrFull("***CHEATING***");
                    outchar('\n');
                    os_PutStrFull("Enter score (int):");
                    os_GetStringInput("", out, 25);
                    score = atoi(out);
                    setHighscore(false, true);
                }
            }
            return 0;
        }
    }

    var = os_GetAppVarData("ROBOTSst", NULL);
    if (var == NULL)
    {
        var = os_CreateAppVar("ROBOTSst", sizeof(settings));
    }
    memcpy(var->data, &settings, sizeof(settings));
    SCALE = settings.SCALE;
    FIRSTWAVE = settings.FIRSTWAVE;
    LEVELUPADD = settings.LEVELUPADD;

    updateHighscore();
    robots_len = FIRSTWAVE - LEVELUPADD;

    gfx_Begin();
    gfx_FillScreen(WHITE);
    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTextFGColor(BLACK);

    sk_key_t key = 0;
    while (!dead)
    {
        if (win)
        {
            win = false;
            setRobots(robots_len + LEVELUPADD);
            current = {(uint24_t)(XMIN + randInt(0, (XMAX - XMIN) / SCALE - 1) * SCALE),
                       (uint16_t)(YMIN + randInt(0, (YMAX - YMIN) / SCALE - 1) * SCALE)};
            updateScreen();
            while (!(key = os_GetCSC()))
                ;
        }
        if (exited)
        {
            boot_WaitShort();
            updateRobots();
            updateScreen();
            if (dead)
            {
                IAmDead();
                while (!os_GetCSC())
                    ;
            }
            if (win)
            {
                IWin();
                exited = false;
            }
        }
        else
        {
            bool wait = true;
            switch (key)
            {
            case sk_Del:
                gfx_End();
                return 0;
            case sk_Clear:
                exited = true;
                break;
            case sk_7:
                wait = move(-1, -1);
                break;
            case sk_8:
            case sk_Up:
                wait = move(0, -1);
                break;
            case sk_9:
                wait = move(1, -1);
                break;
            case sk_4:
            case sk_Left:
                wait = move(-1, 0);
                break;
            case sk_5:
                wait = move(0, 0);
                break;
            case sk_0:
            case sk_Power:
            case sk_Enter:
                teleport();
                break;
            case sk_6:
            case sk_Right:
                wait = move(1, 0);
                break;
            case sk_1:
                wait = move(-1, 1);
                break;
            case sk_2:
            case sk_Down:
                wait = move(0, 1);
                break;
            case sk_3:
                wait = move(1, 1);
                break;
            case sk_Vars:
                showVars();
                break;
            }
            while (!(key = os_GetCSC()) && !exited && wait)
                ;
            if (key == sk_Store && dead)
            {
                dead = false;
            }
        }
    }

    gfx_End();
    return 0;
}
