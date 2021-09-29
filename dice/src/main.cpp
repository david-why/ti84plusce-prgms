#include <tice.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fileioc.h>

int main()
{
    char *str = (char *)malloc(100 * sizeof(char)),
         *last = (char *)malloc(100 * sizeof(char)),
         *str2, *char_d, *char_sign, out[100];
    bool adv = false, disadv = false;
    int a = 1, b = 0, c = 0, n;
    *str = '\0';

    srand(rtc_Time());

    os_ClrHome();

    while (true)
    {
        a = 1, b = 0, c = 0;
        strcpy(last, str);
        os_GetStringInput((char *)">", str, 100);
        os_NewLine();
        if (!strcmp(str, "/"))
        {
            break;
        }

        if (!strlen(str))
        {
            strcpy(str, last);
        }

        if ((char_d = strchr(str, '.')) == NULL)
        {
            char_d = strchr(str, 'D');
        }

        // Set number of dice here.
        if (char_d)
        {
            a = atoi(str);
            if (!a)
            {
                a = 1;
            }

            str2 = char_d + 1;
        }
        else
        {
            str2 = str;
        }
        if (!strlen(str2))
        {
            continue;
        }

        // Set dice sides here.
        b = atoi(str2);
        if (!b)
        {
            continue;
        }

        if ((char_sign = strchr(str2, '+')) != NULL)
        {
            c = atoi(char_sign + 1);
        }
        if ((char_sign = strchr(str2, '-')) != NULL)
        {
            c = -atoi(char_sign + 1);
        }

        if (strchr(str, '('))
        {
            adv = true;
        }
        if (strchr(str, ')'))
        {
            disadv = true;
        }

        n = c;
        for (long i = 0; i < a; i++)
        {
            n += randInt(1, b);
        }
        if (adv)
        {
            int d = c;
            for (long i = 0; i < a; i++)
            {
                d += randInt(1, b);
            }
            if (d > n)
            {
                n = d;
            }
        }
        if (disadv)
        {
            int d = c;
            for (long i = 0; i < a; i++)
            {
                d += randInt(1, b);
            }
            if (d < n)
            {
                n = d;
            }
        }

        sprintf(out, "%d", n);
        os_PutStrFull(out);

        os_NewLine();
    }
    free(str);
    free(last);

    return 0;
}
