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

constexpr uint8_t cols = LCD_WIDTH / 8, rows = LCD_HEIGHT / 8;

char data[rows][cols], buf[cols + 1];
uint8_t selrow, selcol;
uint8_t highlight[cols][2], hllen;

bool partial;
uint8_t prt[cols][2], prtlen;

const char *chars = "           WRMH    VQLG   ZUPKFC  YTOJEB  XSNIDA        ";

void setupgfx()
{
    gfx_Begin();
    gfx_SetMonospaceFont(8);
    gfx_SetTextTransparentColor(1);
    gfx_SetDrawBuffer();
    partial = false;
}

void inputword()
{
    uint8_t len = 0;
    buf[0] = 0;
    while (true)
    {
        gfx_FillScreen(255);
        gfx_SetTextScale(2, 2);
        gfx_PrintStringXY("Word?", 50, 50);
        gfx_SetTextScale(1, 1);
        gfx_PrintStringXY(buf, 50, 100);
        gfx_SwapDraw();
        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Enter)
            break;
        if (key == sk_Clear)
        {
            len = 0;
            buf[0] = 0;
        }
        else if (key == sk_Del && len > 0)
            buf[--len] = 0;
        else if (chars[key] != ' ' && len < sizeof(buf) - 1)
            buf[len++] = chars[key], buf[len] = 0;
    }
    gfx_FillScreen(255);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Searching...", 50, 50);
    gfx_SetTextScale(1, 1);
    gfx_SwapDraw();
    partial = false;
}

void dochk()
{
    inputword();
    hllen = strlen(buf);

    // horizontal
    for (uint8_t r = 0; r < rows; r++)
    {
        for (uint8_t c = 0; c <= cols - hllen; c++)
        {
            // left-to-right
            bool found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r;
                highlight[i][1] = c + i;
                if (data[r][c + i] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
            // right-to-left
            found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r;
                highlight[i][1] = c + i;
                if (data[r][c + hllen - i - 1] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
        }
    }
    // vertical
    for (uint8_t c = 0; c < cols; c++)
    {
        for (uint8_t r = 0; r <= rows - hllen; r++)
        {
            // left-to-right
            bool found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r + i;
                highlight[i][1] = c;
                if (data[r + i][c] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
            // right-to-left
            found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r + i;
                highlight[i][1] = c;
                if (data[r + hllen - i - 1][c] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
        }
    }
    // diagonal, \ dir
    for (uint8_t r = 0; r <= rows - hllen; r++)
    {
        for (uint8_t c = 0; c <= cols - hllen; c++)
        {
            // left-to-right
            bool found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r + i;
                highlight[i][1] = c + i;
                if (data[r + i][c + i] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
            // right-to-left
            found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r + i;
                highlight[i][1] = c + i;
                if (data[r + hllen - i - 1][c + hllen - i - 1] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
        }
    }
    // diagonal, / dir
    for (uint8_t r = 0; r <= rows - hllen; r++)
    {
        for (uint8_t c = 0; c <= cols - hllen; c++)
        {
            // left-to-right
            bool found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r + i;
                highlight[i][1] = c + hllen - i - 1;
                if (data[r + hllen - i - 1][c + i] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
            // right-to-left
            found = true;
            for (uint8_t i = 0; i < hllen; i++)
            {
                highlight[i][0] = r + i;
                highlight[i][1] = c + hllen - i - 1;
                if (data[r + i][c + hllen - i - 1] != buf[i])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return;
        }
    }

    hllen = 0;
}

void dodraw(uint8_t r, uint8_t c, bool setpos = true)
{
    if (setpos)
        gfx_SetTextXY(c * 8, r * 8);
    bool hl = false, bg = r == selrow && c == selcol;
    for (uint8_t i = 0; i < hllen; i++)
    {
        if (highlight[i][0] == r && highlight[i][1] == c)
        {
            hl = true;
            break;
        }
    }
    if (bg)
        gfx_SetTextBGColor(240);
    if (hl)
        gfx_SetTextFGColor(16);
    gfx_PrintChar(data[r][c]);
    if (bg)
        gfx_SetTextBGColor(255);
    if (hl)
        gfx_SetTextFGColor(0);
}

void dosto()
{
    ti_CloseAll();
    ti_var_t var = ti_Open("WORDSRCH", "w");
    ti_Write(data, sizeof(data), 1, var);
    ti_Write(&selrow, sizeof(selrow), 1, var);
    ti_Write(&selcol, sizeof(selcol), 1, var);
    ti_Write(highlight, sizeof(highlight), 1, var);
    ti_Write(&hllen, sizeof(hllen), 1, var);
    ti_SetArchiveStatus(true, var);
    ti_Close(var);
}

void doload()
{
    var_t *save = os_GetAppVarData("WORDSRCH", NULL);
    if (save == NULL)
        return;
    uint8_t *ptr = save->data;
    memcpy(data, ptr, sizeof(data));
    ptr += sizeof(data);
    selrow = *(ptr++);
    selcol = *(ptr++);
    memcpy(highlight, ptr, sizeof(highlight));
    ptr += sizeof(highlight);
    hllen = *(ptr++);
    ti_Delete("WORDSRCH");
}

bool dostep()
{
    if (!partial)
    {
        gfx_FillScreen(255);
        for (uint8_t r = 0; r < rows; r++)
        {
            gfx_SetTextXY(0, r * 8);
            for (uint8_t c = 0; c < cols; c++)
            {
                dodraw(r, c, false);
            }
        }
        gfx_SwapDraw();
        partial = true;
    }
    else
    {
        gfx_BlitScreen();
        for (uint8_t i = 0; i < prtlen; i++)
            dodraw(prt[i][0], prt[i][1]);
        gfx_SwapDraw();
        prtlen = 0;
    }
    do
        kb_Scan();
    while (!kb_AnyKey());
    if (kb_IsDown(kb_KeyClear))
        return false;
    if (kb_IsDown(kb_KeyMode))
    {
        dosto();
        return false;
    }
    kb_key_t g7 = kb_Data[7];
    if (g7)
    {
        prtlen = 2;
        prt[0][0] = selrow;
        prt[0][1] = selcol;
        if (g7 & kb_Up)
        {
            if (selrow == 0)
                selrow = rows;
            selrow--;
        }
        if (g7 & kb_Down)
        {
            selrow++;
            if (selrow == rows)
                selrow = 0;
        }
        if (g7 & kb_Left)
        {
            if (selcol == 0)
                selcol = cols;
            selcol--;
        }
        if (g7 & kb_Right)
        {
            selcol++;
            if (selcol == cols)
                selcol = 0;
        }
        prt[1][0] = selrow;
        prt[1][1] = selcol;
        delay(50);
    }
    if (kb_IsDown(kb_KeyEnter))
        dochk();
    if (kb_IsDown(kb_KeyDel))
    {
        data[selrow][selcol] = ' ';
        prtlen = 1;
        prt[0][0] = selrow;
        prt[0][1] = selcol;
    }
    sk_key_t key = os_GetCSC();
    if (chars[key] != ' ')
    {
        data[selrow][selcol] = chars[key];
        prtlen = 1;
        prt[0][0] = selrow;
        prt[0][1] = selcol;
    }
    return true;
}

int main()
{
    doload();
    setupgfx();
    while (dostep())
        ;
    gfx_End();
    return 0;
}
