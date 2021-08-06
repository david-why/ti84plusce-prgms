#include <stdio.h>
#include <tice.h>

/**
 * Inverts the text output.
 */
void invert()
{
    asm("   ld   hl, 0D00085h\n   ld   a, 08\n   xor   (hl)\n   ld   (hl), a");
    delay(5);
}

/**
 * Displays an TI-OS like menu for the user to select from.
 * 
 * @param title The title of the menu. Should be a null-terminated string.
 * @param names The names to display. Should be null-terminated strings.
 * @param items The length of \p names.
 * @returns The index chosen, starting from 1, or \c 0 if the user pressed CLEAR.
 */
uint8_t menu(const char *title, const char *names[], uint8_t items)
{
    if (items == 0)
        return 0;
    uint8_t selected = 1;
    uint8_t begin = 1;
    bool flush = true;
    while (true)
    {
        if (flush)
        {
            flush = false;
            os_ClrHome();
            invert();
            os_PutStrLine(title);
            invert();
        }
        os_SetCursorPos(1, 0);
        for (uint8_t prindex = begin; prindex <= ((begin + 8 < items) ? (begin + 8) : items); prindex++)
        {
            char ai = ':';
            if (prindex == begin && begin != 1)
                ai = '\x1e';
            else if (prindex == begin + 8 && prindex != items)
                ai = '\x1f';
            if (selected == prindex)
                invert();
            char outbuf[2];
            if (prindex == 10)
                outbuf[0] = '0';
            else if (prindex < 10)
                sprintf(outbuf, "%u", prindex);
            else
                outbuf[0] = ' ';
            outbuf[1] = 0;
            os_PutStrLine(outbuf);
            outbuf[0] = ai;
            os_PutStrLine(outbuf);
            if (selected == prindex)
                invert();
            os_PutStrLine(names[prindex - 1]);
            unsigned int r, c;
            os_GetCursorPos(&r, &c);
            os_SetCursorPos(r + 1, 0);
        }

        sk_key_t key;
        while (!(key = os_GetCSC()))
            ;
        switch (key)
        {
        case sk_Clear:
            os_ClrHome();
            return 0;
        case sk_Enter:
            os_ClrHome();
            return selected;
        case sk_Down:
            selected++;
            if (selected > items)
            {
                if (begin != 1)
                {
                    begin = 1;
                    flush = true;
                }
                selected = 1;
            }
            else if (selected > begin + 8)
            {
                begin++;
                flush = true;
            }
            break;
        case sk_Up:
            selected--;
            if (selected == 0)
            {
                selected = items;
                if (selected > begin + 8)
                {
                    begin = (selected - 8 > 0 ? selected - 8 : 1);
                    flush = true;
                }
            }
            else if (selected < begin)
            {
                begin--;
                flush = true;
            }
            break;
        case sk_1:
            os_ClrHome();
            return 1;
        case sk_2:
            if (items > 1)
            {
                os_ClrHome();
                return 2;
            }
            break;
        case sk_3:
            if (items > 2)
            {
                os_ClrHome();
                return 3;
            }
            break;
        case sk_4:
            if (items > 3)
            {
                os_ClrHome();
                return 4;
            }
            break;
        case sk_5:
            if (items > 4)
            {
                os_ClrHome();
                return 5;
            }
            break;
        case sk_6:
            if (items > 5)
            {
                os_ClrHome();
                return 6;
            }
            break;
        case sk_7:
            if (items > 6)
            {
                os_ClrHome();
                return 7;
            }
            break;
        case sk_8:
            if (items > 7)
            {
                os_ClrHome();
                return 8;
            }
            break;
        case sk_9:
            if (items > 8)
            {
                os_ClrHome();
                return 9;
            }
            break;
        case sk_0:
            if (items > 9)
            {
                os_ClrHome();
                return 10;
            }
            break;
        }
    }
}

/**
 * Gets the user to input a floating point number.
 * 
 * @param prompt The prompt to display to the user.
 * @param buf An input buffer. A size of at least 50 is recommended.
 * @param size The size of the \p buf.
 * @returns The number the user inputted.
 */
float numericInput(const char *prompt, char *buf, size_t size)
{
    os_GetStringInput((char *)prompt, buf, size);
    real_t ans = os_StrToReal(buf, NULL);
    return os_RealToFloat(&ans);
}
