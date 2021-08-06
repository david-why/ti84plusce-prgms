#include <tice.h>

int main()
{
    os_ClrHome();

    os_PutStrFull("TESTING!!! THIS SHOULD DO textwrappppp");

    while (!os_GetCSC())
    {
        ;
    }

    return 0;
}
