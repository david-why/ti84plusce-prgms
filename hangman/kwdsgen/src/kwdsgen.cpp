#include <string.h>
#include <stdio.h>

#include <tice.h>
#include <fileioc.h>

#define WORDLENGTH 11
#define KEYWORDSLENGTH 1000

#include "kwds.h"

const uint16_t test = 12345;

char buf[WORDLENGTH + 1];
char strbuf[5];
int main()
{
    ti_CloseAll();
    ti_var_t var = ti_Open(packname, "w");
    ti_Write(&test, 2, 1, var);
    ti_Write(&kwdslength, 2, 1, var);
    for (uint16_t i = 0; i < kwdslength; i++)
    {
        os_HomeUp();
        sprintf(strbuf, "%u", i + 1);
        os_PutStrFull(strbuf);
        size_t len = strlen(keywords[i]);
        ti_Write(&len, 1, 1, var);
        strcpy(buf, keywords[i]);
        ti_Write(buf, WORDLENGTH, 1, var);
    }

    ti_SetArchiveStatus(true, var);
    ti_CloseAll();

    os_ClrHome();
    os_PutStrFull("Finished!");

    while (!os_GetCSC())
        ;

    return 0;
}
