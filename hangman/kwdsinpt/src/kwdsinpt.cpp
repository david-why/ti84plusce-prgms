#include <stdio.h>
#include <string.h>

#include <tice.h>
#include <fileioc.h>

#define WORDLENGTH 11
#define KEYWORDSLENGTH 1000

typedef struct
{
    uint8_t length;
    char word[WORDLENGTH];
} keyword_t;

typedef struct
{
    uint16_t length;
    keyword_t words[KEYWORDSLENGTH];
} keywords_t;

char outbuf[25];
char inbuf[WORDLENGTH + 1];

int main()
{
    os_ClrHome();

    ti_CloseAll();
    ti_var_t tvar = ti_Open("HNGMNWRD", "w");
    ti_Write(outbuf, 2, 1, tvar);
    uint16_t length = 0;
    os_PutStrFull("Please enter words, any   "
                  "number to end:            ");
    bool cont = true;
    while (cont)
    {
        sprintf(outbuf, "Enter word #%u: ", length + 1);
        os_GetStringInput(outbuf, inbuf, WORDLENGTH);
        bool valid = true;
        for (uint8_t i = 0; i < strlen(inbuf); i++)
        {
            if (inbuf[i] >= '0' && inbuf[i] <= '9')
            {
                valid = false;
                cont = false;
            }
            else if (inbuf[i] < 'A' || inbuf[i] > 'Z')
            {
                valid = false;
            }
        }
        if (valid)
        {
            size_t s = strlen(inbuf);
            ti_Write(&s, 1, 1, tvar);
            ti_Write(inbuf, WORDLENGTH, 1, tvar);
            length++;
        }
        else
        {
            os_PutStrFull("Not valid");
            delay(1000);
        }
        os_ClrHome();
    }

    ti_Rewind(tvar);
    ti_Write(&length, 2, 1, tvar);
    ti_SetArchiveStatus(true, tvar);
    ti_CloseAll();

    return 0;
}
