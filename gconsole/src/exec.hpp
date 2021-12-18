#ifndef _EXEC_HPP
#define _EXEC_HPP

#include "util.hpp"
#include <fileioc.h>
#include <string.h>

int main();
int runprgm_callback(void *, int)
{
    return main();
}

void runprogram(const char *name)
{
    gfx_End();
    os_ClrHome();
    settings *s = get_settings();
    s->reqpwd = false;
    save_settings(s);

    os_RunPrgm(name, NULL, 0, runprgm_callback);
    char buf[9];
    strcpy(buf, name);
    buf[0] &= 63;
    os_RunPrgm(buf, NULL, 0, runprgm_callback);

    // ti_CloseAll();
    // ti_var_t slot = ti_OpenVar(name, "r", TI_PPRGM_TYPE);
    // ti_CloseAll();
    // if (slot)
    // {
    //     setuprun();
    //     // setop1(TI_PPRGM_TYPE, name);
    //     // runasmprgm();
    //     os_RunPrgm(name, NULL, 0, runprgm_callback);
    // }
    // slot = ti_OpenVar(name, "r", TI_PRGM_TYPE);
    // ti_CloseAll();
    // if (slot)
    // {
    //     setuprun();
    //     // setop1(TI_PRGM_TYPE, name);
    //     // runprgm();
    //     os_RunPrgm(name, NULL, 0, runprgm_callback);
    // }

    // if (name[0] & 64)
    // {
    //     char buf[9];
    //     strcpy(buf, name);
    //     buf[0] &= 63;
    //     slot = ti_OpenVar(buf, "r", TI_PPRGM_TYPE);
    //     ti_CloseAll();
    //     if (slot)
    //     {
    //         setuprun();
    //         // setop1(TI_PPRGM_TYPE, buf);
    //         // runasmprgm();
    //         os_RunPrgm(buf, NULL, 0, runprgm_callback);
    //     }
    //     slot = ti_OpenVar(buf, "r", TI_PRGM_TYPE);
    //     ti_CloseAll();
    //     if (slot)
    //     {
    //         setuprun();
    //         // setop1(TI_PRGM_TYPE, buf);
    //         // runprgm();
    //         os_RunPrgm(buf, NULL, 0, runprgm_callback);
    //     }
    // }
    // save_settings(s);
}

#endif /* _EXEC_HPP */