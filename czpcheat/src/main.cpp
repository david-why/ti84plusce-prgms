#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <tice.h>
#include <fileioc.h>
#include <string.h>

struct save
{
    struct single
    {
        char name[26];
        char score[6];
    } entries[12];
};

char buf[128];

int main()
{
    os_ClrHome();
    os_GetStringInput((char *)"Name?", buf, 26);
    os_NewLine();
    size_t name_len = strlen(buf);
    if (name_len == 0)
    {
        ti_Delete("CZapHigh");
        os_PutStrFull("Reset highscore!");
        delay(1000);
        return 0;
    }
    save s;
    ti_CloseAll();
    ti_var_t var = ti_Open("CZapHigh", "r");
    ti_Read(&s, sizeof(save), 1, var);
    ti_Close(var);

    for (uint8_t i = 0; i < 12; i++)
    {
        strcpy(s.entries[i].name, buf);
        for (uint8_t j = name_len; j < 26; j++)
            s.entries[i].name[j] = ' ';
        strncpy(s.entries[i].score, "999999", 6);
    }

    ti_CloseAll();
    var = ti_Open("CZapHigh", "w");
    ti_Write(&s, sizeof(save), 1, var);
    ti_Close(var);

    os_PutStrFull("Done!");
    delay(1000);

    return 0;
}
