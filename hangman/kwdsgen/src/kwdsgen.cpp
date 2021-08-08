#include <string.h>
#include <tice.h>
#include <fileioc.h>

#define WORDLENGTH 11
#define KEYWORDSLENGTH 1000

#include "kwds.h"

char buf[WORDLENGTH + 1];
int main()
{
    os_ClrHome();

    ti_CloseAll();
    ti_var_t var = ti_Open("HNGMNWRD", "w");
    ti_Write(&kwdslength, 2, 1, var);
    for (uint16_t i = 0; i < kwdslength; i++)
    {
        size_t len = strlen(keywords[i]);
        ti_Write(&len, 1, 1, var);
        strcpy(buf, keywords[i]);
        ti_Write(buf, WORDLENGTH, 1, var);
    }

    ti_SetArchiveStatus(true, var);
    ti_CloseAll();

    os_PutStrFull("Finished!");

    while (!os_GetCSC())
        ;

    return 0;
}
