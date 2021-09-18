#include <tice.h>
#include <fileioc.h>

const char hexadecimal[] = "0123456789ABCDEF";
char buf[3];

constexpr uint8_t getchar(uint8_t i, uint8_t j)
{
    uint8_t index = i * 16 + j;
    if (index == 0xD6 || index == 0x00)
        index = 0x20;
    return index;
}

void putindex(uint8_t i, uint8_t j)
{
    buf[0] = getchar(i, j);
    buf[1] = 0;
    os_PutStrLine(buf);
}

void show(uint8_t i, uint8_t minus = 0)
{
    os_SetCursorPos(i + 1 - minus, 0);
    buf[0] = hexadecimal[i];
    buf[1] = ' ';
    buf[2] = 0;
    os_PutStrLine(buf);
    for (uint8_t j = 0; j < 16; j++)
    {
        putindex(i, j);
    }
}

int main()
{
    uint8_t anstype;
    var_t *ans = (var_t *)os_GetAnsData(&anstype);
    uint8_t toconvert = 0;
    do
    {
        if (anstype == TI_STRING_TYPE)
        {
            string_t *str = (string_t *)ans;
            if (str->len == 2)
            {
                char first = str->data[0];
                if (first >= 'A')
                    first -= '7';
                else
                    first -= '0';
                if (first < 0 || first > 15)
                    break;
                char second = str->data[1];
                if (second >= 'A')
                    second -= '7';
                else
                    second -= '0';
                if (second < 0 || second > 15)
                    break;
                uint8_t ch = getchar(first, second);
                toconvert = ch;
            }
        }
        else if (anstype == TI_REAL_TYPE)
        {
            int val = os_RealToInt24((real_t *)ans);
            if (val > 0 && val < 0xF8)
                toconvert = getchar(0, val);
        }
    } while (0);
    if (toconvert > 0)
    {
        buf[0] = toconvert;
        ti_CloseAll();
        uint8_t slot = ti_OpenVar(ti_Str1, "a", TI_STRING_TYPE);
        if (!slot)
            return 1;
        ti_Write(buf, 1, 1, slot);
        ti_CloseAll();
        return 0;
    }

    os_ClrHome();
    os_SetCursorPos(0, 2);
    os_PutStrLine(hexadecimal);
    for (uint8_t i = 0; i < 8; i++)
        show(i);
    os_SetCursorPos(2, 20);
    os_PutStrLine("Press");
    os_SetCursorPos(3, 21);
    os_PutStrLine("Any");
    os_SetCursorPos(4, 21);
    os_PutStrLine("Key");
    os_SetCursorPos(5, 21);
    os_PutStrLine("For");
    os_SetCursorPos(6, 19);
    os_PutStrLine("More...");
    while (!os_GetCSC())
        delay(10);
    os_ClrHome();
    os_SetCursorPos(0, 2);
    os_PutStrLine(hexadecimal);
    for (uint8_t i = 8; i < 16; i++)
        show(i, 8);
    while (!os_GetCSC())
        delay(10);

    return 0;
}
