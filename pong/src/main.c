#include "condrive.h"
#include <tice.h>
#include <stdio.h>
con_device_t con;
int val = 0, run = 1;
static void putint(int i)
{
    char buf[16];
    real_t r = os_Int24ToReal(i);
    os_RealToStr(buf, &r, 0, 0, 0);
    os_PutStrFull(buf);
}
void handle_up(con_device_t *con __attribute__((__unused__))) { val++; }
void handle_down(con_device_t *con __attribute__((__unused__))) { val--; }
void handle_quit(con_device_t *con __attribute__((__unused__))) { run = 0; }
int main()
{
    os_ClrHome();
    puts("Please connect device...");
    con_error_t err = con_Init(&con, 10);
    if (err)
    {
        puts("con_Init error ");
        putint(err);
        os_GetKey();
        return 1;
    }
    con_RegisterCallback(&con, 'U', handle_up);
    con_RegisterCallback(&con, 'D', handle_down);
    con_RegisterCallback(&con, 'Q', handle_quit);
    puts("Device connected!");
    while (run)
    {
        con_HandleEvents(&con);
        os_SetCursorPos(2, 0);
        putint(val);
        os_PutStrLine("         ");
        sk_key_t key = os_GetCSC();
        if (key == sk_Clear)
            break;
        else if (key == sk_Up)
            con_EmitEvent(&con, 'U', NULL, 0), val++;
        else if (key == sk_Down)
            con_EmitEvent(&con, 'D', NULL, 0), val--;
        else if (key == sk_Clear)
            con_EmitEvent(&con, 'Q', NULL, 0), run = 0;
    }
    con_Deinit(&con);
    return 0;
}
