#include <tice.h>
#include <graphx.h>
#include <string.h>

int getinput(char *buffer,
             unsigned int maxsize,
             int x,
             int y,
             sk_key_t okkey = sk_Enter,
             sk_key_t quitkey = sk_Mode,
             const char *var_n = NULL,
             const char *var_s = NULL,
             const char *var_A = NULL,
             const char *var_a = NULL)
{
    if (maxsize-- < 2)
        return 0;
    uint8_t col = gfx_SetColor(255);
    unsigned int pos = 0;
    buffer[0] = 0;
    bool second = false, alock = false;
    uint8_t alpha = 0;
    sk_key_t key;
    while (true)
    {
        gfx_BlitScreen();
        gfx_FillRectangle(x, y, LCD_WIDTH, LCD_HEIGHT);
        gfx_FillRectangle(0, 0, 16, 8);
        if (second)
        {
            gfx_SetTextXY(0, 0);
            gfx_PrintChar('S');
        }
        if (alpha == 1)
        {
            gfx_SetTextXY(8, 0);
            gfx_PrintChar('A');
        }
        if (alpha == 2)
        {
            gfx_SetTextXY(8, 0);
            gfx_PrintChar('a');
        }
        gfx_SetTextXY(x, y);
        for (unsigned int i = 0; i < pos; i++)
        {
            gfx_PrintChar(buffer[i]);
            if (i && i % 35 == 0)
                gfx_SetTextXY(x, y + i / 35 * 12);
        }
        gfx_SwapDraw();
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == okkey)
        {
            buffer[pos] = 0;
            gfx_SetColor(col);
            return pos;
        }
        if (key == quitkey)
        {
            buffer[0] = 0;
            gfx_SetColor(col);
            return -1;
        }
        auto c = [&pos, maxsize, &alpha, alock, &second, buffer](char n, char s, char A, char a)
        {
            if (pos >= maxsize)
                return;
            if (alpha == 2)
            {
                if (!a)
                    return;
                if (!alock)
                    alpha = 0;
                buffer[pos++] = a;
                return;
            }
            if (alpha == 1)
            {
                if (!A)
                    return;
                if (!alock)
                    alpha = 0;
                buffer[pos++] = A;
                return;
            }
            if (second)
            {
                if (!s)
                    return;
                second = false;
                buffer[pos++] = s;
                return;
            }
            if (n)
                buffer[pos++] = n;
            return;
        };
        switch (key)
        {
        case sk_Clear:
            pos = 0;
            buffer[0] = 0;
            break;
        case sk_2nd:
            second = !second;
            break;
        case sk_Del:
            if (pos != 0)
                buffer[--pos] = 0;
            break;
        case sk_Alpha:
            if (second)
                if (alock)
                {
                    alpha = 0;
                    alock = second = false;
                }
                else
                {
                    alock = !(second = false);
                    if (alpha == 0)
                        alpha = 1;
                }
            else if (alock)
            {
                if (++alpha == 3)
                {
                    alock = false;
                    alpha = 0;
                }
            }
            else if (++alpha == 3)
                alpha = 0;
            break;
        case sk_GraphVar:
            c('X', 0, 0, 0);
            break;
        case sk_Math:
            c(0, 0, 'A', 'a');
            break;
        case sk_Apps:
            c(0, 0, 'B', 'b');
            break;
        case sk_Prgm:
            c(0, 0, 'C', 'c');
            break;
        case sk_Recip:
            c(0, 0, 'D', 'd');
            break;
        case sk_Sin:
            c(0, 0, 'E', 'e');
            break;
        case sk_Cos:
            c(0, 0, 'F', 'f');
            break;
        case sk_Tan:
            c(0, 0, 'G', 'g');
            break;
        case sk_Power:
            c('^', 0, 'H', 'h');
            break;
        case sk_Square:
            c(0, 0, 'I', 'i');
            break;
        case sk_Comma:
            c(',', 0, 'J', 'j');
            break;
        case sk_LParen:
            c('(', '{', 'K', 'k');
            break;
        case sk_RParen:
            c(')', '}', 'L', 'l');
            break;
        case sk_Div:
            c('/', 'e', 'M', 'm');
            break;
        case sk_Log:
            c(0, 0, 'N', 'n');
            break;
        case sk_7:
            c('7', 'u', 'O', 'o');
            break;
        case sk_8:
            c('8', 'v', 'P', 'p');
            break;
        case sk_9:
            c('9', 'w', 'Q', 'q');
            break;
        case sk_Mul:
            c('*', '[', 'R', 'r');
            break;
        case sk_Ln:
            c(0, 0, 'S', 's');
            break;
        case sk_4:
            c('4', 0, 'T', 't');
            break;
        case sk_5:
            c('5', 0, 'U', 'u');
            break;
        case sk_6:
            c('6', 0, 'V', 'v');
            break;
        case sk_Sub:
            c('-', ']', 'W', 'w');
            break;
        case sk_Store:
            c('>', 0, 'X', 'x');
            break;
        case sk_1:
            c('1', 0, 'Y', 'y');
            break;
        case sk_2:
            c('2', 0, 'Z', 'z');
            break;
        case sk_3:
            c('3', 0, 0, 0);
            break;
        case sk_Add:
            c('+', 0, '"', '\'');
            break;
        case sk_0:
            c('0', 0, ' ', ' ');
            break;
        case sk_DecPnt:
            c('.', 'i', ':', ';');
            break;
        case sk_Chs:
            c('_', 0, '?', '!');
            break;
        case sk_Yequ:
            c(0, 0, '@', '#');
            break;
        case sk_Window:
            c(0, 0, '$', '%');
            break;
        case sk_Zoom:
            c(0, 0, '|', '\\');
            break;
        case sk_Trace:
            c(0, 0, '<', '>');
            break;
        case sk_Vars:
            c(1, 2, 3, 4);
            break;
        }
        auto v = [&pos, maxsize, buffer](const char *str, const char *repl)
        {
            size_t len = strlen(str);
            size_t repllen = strlen(repl);
            if (pos < len || pos - len + repllen >= maxsize || strncmp(buffer + (pos - len), str, len))
                return;
            pos -= len;
            strcpy(buffer + pos, repl);
            pos += repllen;
        };
        v("_eq_", "=");
        v("_gt_", ">");
        v("_lt_", "<");
        v("_and_", "&");
        v("_exc_", "!");
        v("_at_", "@");
        v("_hash_", "#");
        v("_money_", "$");
        v("_pc_", "%");
        v("_xor_", "^");

        v("\1", var_n ? var_n : "");
        v("\2", var_s ? var_s : "");
        v("\3", var_A ? var_A : "");
        v("\4", var_a ? var_a : "");
    }
}
