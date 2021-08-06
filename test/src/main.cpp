#include <tice.h>
#include <stdio.h>
#include <string.h>
#include <fileioc.h>

int main()
{
    os_ClrHome();
    char out[5];
    sk_key_t k;

    while (!(k = os_GetCSC()))
        ;

    sprintf(out, "%d", k);
    os_PutStrFull(out);
    while (!os_GetCSC())
        ;

    return 0;
}