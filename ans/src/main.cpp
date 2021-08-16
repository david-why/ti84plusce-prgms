#include <tice.h>
#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>

#include <stdio.h>
#include <string.h>

#include "gfx_add.cpp"

char strbuf[513];
char small_strbuf[129];

uint8_t space[2 + 16 * 16] = {16, 16};
gfx_sprite_t *buf = (gfx_sprite_t *)space;

sk_key_t messagebox(const char *title, const char *content, uint8_t color = 0)
{
    uint8_t prevtcolor = gfx_SetTextFGColor(color);
    uint8_t prevdcolor = gfx_SetColor(0);
    uint8_t pos = gfx_GetDraw();
    if (pos == gfx_buffer)
        gfx_SetDrawScreen();
    gfx_BlitScreen();

    gfx_FillScreen(255);
    unsigned int title_len = strlen(title);
    unsigned int content_len = strlen(content);
    unsigned int title_width = title_len * 16;
    unsigned int content_width = content_len * 8;
    unsigned int width = title_width > content_width ? title_width : content_width;
    if (width > 256)
        width = 256;
    unsigned int title_height = title_width / width;
    if (title_width % width != 0)
        title_height++;
    unsigned int content_height = content_width / width;
    if (content_width % width != 0)
        content_height++;
    width += 16;
    unsigned int height = 2 + title_height * 16 + 2 + 5 + 2 + content_height * 8 + 2;
    unsigned int x = (LCD_WIDTH - width) / 2;
    unsigned int y = (LCD_HEIGHT - height) / 2;
    gfx_Rectangle(x, y, width, height);
    gfx_HorizLine(x, y + 2 + title_height * 16 + 2, width);

    char b[2];
    b[1] = 0;
    for (uint8_t line = 0; line < title_height; line++)
    {
        for (uint8_t char_index = 0; char_index < 16; char_index++)
        {
            unsigned int real_index = line * 16 + char_index;
            if (real_index >= title_len)
                break;
            b[0] = title[real_index];
            gfx_ScaledTextXY(b, x + 8 + char_index * 16, y + 2 + line * 16, 2, buf);
        }
    }
    for (uint8_t line = 0; line < content_height; line++)
    {
        for (uint8_t char_index = 0; char_index < 32; char_index++)
        {
            unsigned int real_index = line * 32 + char_index;
            if (real_index >= content_len)
                break;
            b[0] = content[real_index];
            gfx_PrintStringXY(b, x + 8 + char_index * 8, y + 2 + title_height * 16 + 2 + 5 + line * 8);
        }
    }

    gfx_SetTextFGColor(prevtcolor);
    gfx_SetColor(prevdcolor);

    sk_key_t key;
    while (!(key = os_GetCSC()))
        delay(10);

    if (pos == gfx_buffer)
        gfx_SetDrawBuffer();
    gfx_BlitBuffer();

    return key;
}

#define errorbox(title, content) messagebox(title, content, 224)
#define warningbox(title, content) messagebox(title, content, 230)
#define infobox(title, content) messagebox(title, content, 0)

template <uint8_t len, size_t buflen>
char *text4menu(const char *(&text)[len], char (&strbuf)[buflen], uint8_t selected)
{
    uint8_t lines = (buflen - 1) / 32;
    if (lines < len)
        return NULL;
    uint8_t line = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        size_t lenstr = strlen(text[i]);
        uint8_t takelines = 1 + (lenstr + 1) / 32;
        if (line + takelines > lines)
            return NULL;
        if (selected == i)
            strbuf[line * 32] = '>';
        else
            strbuf[line * 32] = ' ';
        strbuf[line * 32 + 1] = ' ';
        strcpy(strbuf + line * 32 + 2, text[i]);
        line += takelines;
        for (uint8_t j = (lenstr + 2) % 32; j < 32 && j; j++)
        {
            strbuf[line * 32 - 32 + j] = ' ';
        }
    }
    return strbuf;
}

template <size_t len>
uint8_t menu(const char *title, const char *(&text)[len])
{
    if (len == 1)
        return 0;
    uint8_t selected = 1;
    sk_key_t key;
    bool run = true;
    while (run)
    {
        key = messagebox(title, text4menu(text, strbuf, selected - 1), 0);
        switch (key)
        {
        case sk_Enter:
            run = false;
            break;
        case sk_Up:
            selected--;
            if (selected == 0)
                selected = len;
            break;
        case sk_Down:
            selected++;
            if (selected > len)
                selected = 1;
            break;
        }
    }
    return selected - 1;
}

const char *sel_action = "Please select:  ";
const char *real_actions[] = {"Factoralize", "DONT CHOOSE"};

void real_fact(real_t *var)
{
    do
        kb_ScanGroup(6);
    while (kb_IsDown(kb_KeyEnter));

    real_t zero = os_Int24ToReal(0);
    if (os_RealCompare(var, &zero) < 0)
    {
        errorbox("Error", "The variable Ans is negative!");
        return;
    }

    gfx_ScaledTextXY("Fact(Ans):", 10, 10, 2, buf);
    gfx_BlitBuffer();
    real_t one = os_Int24ToReal(1);
    real_t two = os_Int24ToReal(2);
    real_t five_hundred_and_one = os_Int24ToReal(501);
    real_t val = *var;
    gfx_SetColor(255);
    unsigned int y = 30;
    real_t i = os_Int24ToReal(2);
    uint8_t entered = 0;
    for (; os_RealCompare(&i, &val) <= 0; i = os_RealAdd(&i, (entered == 1) ? (&one) : (&two)))
    {
        if (entered == 0)
            entered = 1;
        else if (entered == 1)
            entered = 2;
        real_t mod = os_RealMod(&i, &five_hundred_and_one);
        if (os_RealCompare(&mod, &zero) == 0)
        {
            gfx_FillRectangle_NoClip(200, 10, 120, 8);
            os_RealToStr(strbuf, &i, 15, 0, -1);
            gfx_PrintStringXY(strbuf, 200, 10);
            gfx_BlitBuffer();
        }

        unsigned int count = 0;
        mod = os_RealMod(&val, &i);
        while (os_RealCompare(&mod, &zero) == 0)
        {
            val = os_RealDiv(&val, &i);
            mod = os_RealMod(&val, &i);
            count++;
            kb_ScanGroup(6);
            if (kb_IsDown(kb_KeyClear) || kb_On)
                return;
        }
        if (count)
        {
            gfx_SetTextXY(10, y);
            os_RealToStr(strbuf, &i, 38, 0, -1);
            gfx_PrintString(strbuf);
            gfx_PrintChar('^');
            gfx_PrintUInt(count, 0);
            gfx_FillRectangle_NoClip(200, 18, 120, 8);
            os_RealToStr(strbuf, &val, 13, 0, -1);
            gfx_PrintStringXY("n=", 200, 18);
            gfx_PrintString(strbuf);
            gfx_BlitBuffer();
            y += 8;
        }

        kb_ScanGroup(6);
        if (kb_IsDown(kb_KeyClear) || kb_On)
            return;
        if (kb_IsDown(kb_KeyEnter))
        {
            do
                kb_ScanGroup(6);
            while (kb_IsDown(kb_KeyEnter));

            sk_key_t key = 0;
            char input[21] = {0};
            uint8_t inputted = 0;
            gfx_PrintStringXY("i val:", 0, LCD_HEIGHT - 8);
            gfx_SetColor(0);
            gfx_FillRectangle(56, LCD_HEIGHT - 8, 8, 8);
            gfx_SetColor(255);
            gfx_BlitRectangle(gfx_buffer, 0, LCD_HEIGHT - 8, 64, 8);
            bool run = true;
            bool set = true;
            while (run)
            {
                while (!(key = os_GetCSC()))
                    delay(10);
                switch (key)
                {
                case sk_Clear:
                    set = false;
                case sk_Enter:
                    run = false;
                    break;
#pragma region numbers
                case sk_0:
                    if (inputted != 20)
                        input[inputted++] = '0';
                    break;
                case sk_1:
                    if (inputted != 20)
                        input[inputted++] = '1';
                    break;
                case sk_2:
                    if (inputted != 20)
                        input[inputted++] = '2';
                    break;
                case sk_3:
                    if (inputted != 20)
                        input[inputted++] = '3';
                    break;
                case sk_4:
                    if (inputted != 20)
                        input[inputted++] = '4';
                    break;
                case sk_5:
                    if (inputted != 20)
                        input[inputted++] = '5';
                    break;
                case sk_6:
                    if (inputted != 20)
                        input[inputted++] = '6';
                    break;
                case sk_7:
                    if (inputted != 20)
                        input[inputted++] = '7';
                    break;
                case sk_8:
                    if (inputted != 20)
                        input[inputted++] = '8';
                    break;
                case sk_9:
                    if (inputted != 20)
                        input[inputted++] = '9';
                    break;
#pragma endregion numbers
                }
                gfx_FillRectangle_NoClip(0, LCD_HEIGHT - 8, 64 + inputted * 8, 8);
                gfx_PrintStringXY("i val: ", 0, LCD_HEIGHT - 8);
                gfx_PrintString(input);
                int x = gfx_GetTextX();
                if (inputted != 20)
                {
                    // gfx_PrintChar('\xf1');
                    gfx_SetColor(0);
                    gfx_FillRectangle(x, LCD_HEIGHT - 8, 8, 8);
                    gfx_SetColor(255);
                }
                gfx_BlitRectangle(gfx_buffer, 0, LCD_HEIGHT - 8, 64 + inputted * 8, 8);
            }
            if (set && inputted)
            {
                char *end = input + inputted;
                i = os_StrToReal(input, &end);
            }

            do
                kb_ScanGroup(6);
            while (kb_IsDown(kb_KeyEnter) || kb_IsDown(kb_KeyClear));
            gfx_FillRectangle_NoClip(0, LCD_HEIGHT - 8, 64 + inputted * 8, 8);
            gfx_BlitRectangle(gfx_buffer, 0, LCD_HEIGHT - 8, 64 + inputted * 8, 8);
        }
    }
    gfx_FillRectangle_NoClip(200, 10, 120, 16);
    gfx_SetTextFGColor(240);
    gfx_ScaledTextXY("DONE!", 200, 10, 2, buf);
    gfx_BlitBuffer();

    while (!os_GetCSC())
        delay(10);
    return;
}

void real_main(real_t *var)
{
    strcpy(small_strbuf, sel_action);
    small_strbuf[16] = 'A';
    small_strbuf[17] = '=';
    os_RealToStr(small_strbuf + 18, var, 14, 0, -1);
    switch (menu(small_strbuf, real_actions))
    {
    case 0:
        real_fact(var);
        return;
    }
    return;
}

void string_main(string_t *var)
{
    return;
}

int main()
{
    uint8_t ans_type;
    var_t *var = (var_t *)os_GetAnsData(&ans_type);
    if (ans_type != TI_REAL_TYPE && ans_type != TI_STRING_TYPE)
        return 0;

    gfx_Begin();
    gfx_SetTransparentColor(255);
    gfx_SetMonospaceFont(8);
    gfx_FillScreen(255);
    gfx_SetTextConfig(gfx_text_clip);
    gfx_SetDrawBuffer();
    kb_EnableOnLatch();

    switch (ans_type)
    {
    case TI_REAL_TYPE:
        real_main((real_t *)var);
        // case TI_STRING_TYPE:
        //     string_main((string_t *)var);
    }

    gfx_End();
    kb_ClearOnLatch();

    return 0;
}
