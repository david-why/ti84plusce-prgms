#include <tice.h>
#include <fileioc.h>
#include <string.h>
#include <stdio.h>

char buffer[100];

int main()
{
    void *position = NULL;
    char *name;

    os_ClrHome();
    while ((name = ti_DetectVar(&position, NULL, TI_PRGM_TYPE)))
    {
        if (!strcmp(name, "A"))
        {
            os_PutStrFull("YES");
            ti_CloseAll();
            ti_var_t slot = ti_OpenVar(name, "r", TI_PRGM_TYPE);
            if (slot == 0)
                return 1;
            uint16_t size = ti_GetSize(slot);
            sprintf(buffer, "%u", size);
            os_PutStrFull(buffer);
            ti_Read(buffer, size, 1, slot);
            ti_Close(slot);
            os_ClrHome();
            for (uint16_t i = 0; i < size; i++)
            {
                uint8_t tok = buffer[i];
                switch (tok)
                {
                case t2ByteTok:
                    break;
                case tSqrt:
                    os_PutStrFull("\x10(");
                    break;
                case tProg:
                    os_PutStrFull("prgm");
                    break;
                case tAsm:
                    os_PutStrFull("Asm(");
                    break;
                default:
                    os_PutStrFull((char[]){(char)tok, 0});
                    break;
                }
            }
        }
    }

    while (!os_GetCSC())
        delay(10);
    return 0;
}
