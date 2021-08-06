#include <string.h>

#include <tice.h>
#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>

int drawpos[40];
uint8_t speeds[40];
uint8_t length[40];
char characters[40][30];
char orig[] = "1234567890!@#$%&()[]{}\\|;:?qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
char lambda[] = "M;-";
char *chars = orig;

// Flags
bool F_ASYNC = false;
bool F_SAME_PLACE = true;
bool F_RAINBOW = false;
uint8_t FGCOLOR = 6;
uint8_t FGCOLORH = 255;
uint8_t BGCOLOR = 0;
int sleeptime = 1;
bool preve;

char random_char()
{
    return chars[randInt(0, strlen(chars) - 1)];
}

void randomize(uint8_t col)
{
    drawpos[col] = 5 - randInt(0, 15);
    if (F_ASYNC)
        speeds[col] = randInt(1, 4);
    else
        speeds[col] = 2;
    length[col] = randInt(10, 30);
    for (uint8_t row = 0; row < 30; row++)
        characters[col][row] = random_char();
}

void setAsync() { F_ASYNC = !F_ASYNC; }
void setOldStyle() { F_SAME_PLACE = !F_SAME_PLACE; }
void setLambda() { chars = chars == lambda ? orig : lambda; }
void setRainbow() { F_RAINBOW = !F_RAINBOW; }
void setColor1() { FGCOLOR = 192; }
void setColor2() { FGCOLOR = 6; }
void setColor3() { FGCOLOR = 229; }
void setColor4() { FGCOLOR = 25; }
void setColor5() { FGCOLOR = 120; }
void setColor6() { FGCOLOR = 62; }
void setColor7() { FGCOLOR = 255; }

void show_help()
{
    sk_key_t key;
    os_ClrHome();
    os_PutStrFull("This is the classic Matrix"
                  "screensaver.              "
                  "Basic usage:              "
                  "  prgmMATRIX              "
                  "Advanced usage:           "
                  "  \"ARGS\":prgmMATRIX       "
                  "Where each ARG could be:  "
                  "                          "
                  "                          "
                  "(Any=>more, CLEAR=>exit)");
    while (!(key = os_GetCSC()))
        ;
    if (key == sk_Clear)
        return;
    os_ClrHome();
    os_PutStrFull("* A: Asynchronous scroll  "
                  "* O: Use old-style scroll "
                  "* M: Lambda scroll (*)    "
                  "* R: Rainbow mode         "
                  "* 1: Red color            "
                  "* 2: Green color (default)"
                  "* 3: Yellow color         "
                  "* 4: Blue color           "
                  "* 5: Purple color         "
                  "(Any=>more, CLEAR=>exit)");
    while (!(key = os_GetCSC()))
        ;
    if (key == sk_Clear)
        return;
    os_ClrHome();
    os_PutStrFull("* 6: Cyan color           "
                  "* 7: White color=no color "
                  "* H: This help message    "
                  "(*): Not well-implemented "
                  "                          "
                  "                          "
                  "                          "
                  "                          "
                  "                          "
                  "(Any=>exit)");
    while (!os_GetCSC())
        ;
}

int main()
{
    uint8_t type;
    os_GetAnsData(&type);
    if (type == TI_STRING_TYPE)
    {
        string_t *ansvar;
        ti_RclVar(TI_STRING_TYPE, ti_Ans, (void **)&ansvar);
        for (uint8_t i = 0; i < ansvar->len; i++)
        {
            char c = ansvar->data[i];
            switch (c)
            {
            case 'a':
            case 'A':
                setAsync();
                break;
            case 'o':
            case 'O':
                setOldStyle();
                break;
            case 'm':
            case 'M':
                setLambda();
                break;
            case 'r':
            case 'R':
                setRainbow();
                break;
            case '1':
                setColor1();
                break;
            case '2':
                setColor2();
                break;
            case '3':
                setColor3();
                break;
            case '4':
                setColor4();
                break;
            case '5':
                setColor5();
                break;
            case '6':
                setColor6();
                break;
            case '7':
                setColor7();
                break;
            case 'H':
                show_help();
                return 0;
            }
        }
    }
    srand(rtc_Time());
    gfx_Begin();

    gfx_SetMonospaceFont(8);
    gfx_SetTextFGColor(FGCOLOR);
    gfx_SetTextBGColor(BGCOLOR);
    gfx_SetTextTransparentColor(BGCOLOR);
    gfx_SetTextConfig(gfx_text_clip);
    gfx_SetDrawBuffer();

    for (uint8_t col = 0; col < 40; col += 2)
        randomize(col);

    do
    {
        kb_Scan();
        gfx_FillScreen(0);
        for (uint8_t col = 0; col < 40; col += 2)
        {
            int x = col * 8;
            for (int y = drawpos[col]; y >= 0 && y > drawpos[col] - length[col]; y--)
            {
                char c;
                if (F_SAME_PLACE)
                    c = characters[col][y > 29 ? 29 : y];
                else
                    c = characters[col][y - drawpos[col] + length[col]];
                const char cs[] = {c, 0};
                if (F_RAINBOW)
                {
                    gfx_SetTextFGColor(randInt(1, 255));
                }
                else
                {
                    if (y == drawpos[col])
                        gfx_SetTextFGColor(FGCOLORH);
                    else
                        gfx_SetTextFGColor(FGCOLOR);
                }
                gfx_PrintStringXY(cs, x, y * 8);
            }
            drawpos[col] += speeds[col];
            if (drawpos[col] > 30 + length[col])
            {
                randomize(col);
            }
        }
        gfx_BlitBuffer();

        if (!preve)
        {
            if (kb_IsDown(kb_KeyMath))
                setAsync();
            if (kb_IsDown(kb_KeyGraphVar))
                setOldStyle();
            if (kb_IsDown(kb_KeyDiv))
                setLambda();
            if (kb_IsDown(kb_KeyMul))
                setRainbow();
            if (kb_IsDown(kb_Key1))
                setColor1();
            if (kb_IsDown(kb_Key2))
                setColor2();
            if (kb_IsDown(kb_Key3))
                setColor3();
            if (kb_IsDown(kb_Key4))
                setColor4();
            if (kb_IsDown(kb_Key5))
                setColor5();
            if (kb_IsDown(kb_Key6))
                setColor6();
            if (kb_IsDown(kb_Key7))
                setColor7();
            if (kb_IsDown(kb_KeyUp))
            {
                sleeptime += 20;
                if (sleeptime > 2000)
                    sleeptime = 10;
            }
            if (kb_IsDown(kb_KeyDown))
            {
                sleeptime -= 20;
                if (sleeptime < 0)
                    sleeptime = 0;
            }
        }
        preve = kb_IsDown(kb_KeyMath) ||
                kb_IsDown(kb_KeyGraphVar) ||
                kb_IsDown(kb_KeyDiv) ||
                kb_IsDown(kb_KeyMul) ||
                kb_IsDown(kb_Key1) ||
                kb_IsDown(kb_Key2) ||
                kb_IsDown(kb_Key3) ||
                kb_IsDown(kb_Key4) ||
                kb_IsDown(kb_Key5) ||
                kb_IsDown(kb_Key6) ||
                kb_IsDown(kb_Key7) ||
                kb_IsDown(kb_KeyUp) ||
                kb_IsDown(kb_KeyDown);
        delay(sleeptime - ((F_RAINBOW && sleeptime) ? 1 : 0));
    } while (!kb_IsDown(kb_KeyClear));

    gfx_End();

    return 0;
}
