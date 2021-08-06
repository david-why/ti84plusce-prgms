#include <tice.h>
#include <fileioc.h>
#include <string.h>

void newline()
{
    outchar('\n');
}

int main()
{
    uint8_t ans_type;

    os_ClrHome();

    if (!os_GetAnsData(&ans_type))
        return 1;

    string_t *str;
    uint16_t len;
    char *data;
    switch (ans_type)
    {
    case TI_STRING_TYPE:
        os_PutStrFull("ANS is a string");
        newline();
        str = (string_t *)os_GetAnsData(&ans_type);
        os_PutStrFull("DATA ");
        len = str->len;
        data = (char *)malloc(len + 1);
        strncpy(data, str->data, len);
        data[len] = '\0';
        os_PutStrFull(data);
        newline();
        break;
    case TI_REAL_TYPE:
        os_PutStrFull("ANS is a real");
        newline();
        break;
    default:
        char s[100];
        sprintf(s, "ANS is of type %d", ans_type);
        os_PutStrFull(s);
        newline();
    }

    while (!os_GetCSC())
        ;
    return 0;
}
