#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <tice.h>
#include "TINYSTL/string.h"

#define getkey __extension__(        \
    {                                \
        sk_key_t key;                \
        while (!(key = os_GetCSC())) \
            delay(10);               \
        key;                         \
    })

char print_buf[256];

void print()
{
    os_NewLine();
}
void print(const char *str)
{
    os_PutStrFull(str);
}
void print(int i)
{
    real_t r = os_Int24ToReal(i);
    os_RealToStr(print_buf, &r, 0, 0, -1);
    print(print_buf);
}
void print(float f)
{
    real_t r = os_FloatToReal(f);
    os_RealToStr(print_buf, &r, 0, 0, -1);
    print(print_buf);
}

void to_archive(const char *name)
{
    ti_var_t var = ti_Open(name, "r");
    if (var)
    {
        ti_SetArchiveStatus(true, var);
        ti_Close(var);
    }
}
void to_ram(const char *name)
{
    ti_var_t var = ti_Open(name, "r");
    if (var)
    {
        ti_SetArchiveStatus(false, var);
        ti_Close(var);
    }
}

#pragma pack(push, 1)
struct settings
{
    bool protect;
    bool reqpwd;
    uint8_t pwdlen;
    sk_key_t password[7];
    uint8_t prev_sel;
};
#pragma pack(pop)
const char *settings_file = "GCONSOLE";
settings *get_settings()
{
    var_t *data = os_GetAppVarData(settings_file, NULL);
    if (data == NULL)
    {
        data = os_CreateAppVar(settings_file, sizeof(settings));
        if (data == NULL)
            os_ThrowError(OS_E_MEMORY);
    }
    settings *s = new settings;
    memcpy(s, data->data, sizeof(settings));
    return s;
}
void save_settings(settings *s)
{
    ti_Delete(settings_file);
    var_t *data = os_CreateAppVar(settings_file, sizeof(settings));
    if (data == NULL)
        os_ThrowError(OS_E_MEMORY);
    memcpy(data->data, s, sizeof(settings));
    free(s);
    to_archive(settings_file);
}
uint8_t enterpwd(sk_key_t *buf, uint8_t digits)
{
    print("Password:");
    uint8_t got = 0;
    while (got < digits)
    {
        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Enter)
            return got;
        buf[got++] = key;
        print("*");
    }
    return got;
}

int input_int(const char *str, bool gfx = true)
{
    if (gfx)
    {
        gfx_End();
        os_ClrHome();
    }
    os_GetStringInput((char *)str, print_buf, 256);
    print();
    real_t r = os_StrToReal(print_buf, NULL);
    return os_RealToInt24(&r);
}

template <typename First, typename Second>
struct pair
{
    First first;
    Second second;

    pair() : first(), second() {}
    pair(First f, Second s) : first(f), second(s) {}
};
template <typename First, typename Second>
pair<First, Second> make_pair(First f, Second s)
{
    return pair<First, Second>(f, s);
}

uint8_t detect(const char *name)
{
    int arc;
    var_t *var = os_GetAppVarData(name, &arc);
    if (var == NULL)
        return 0;
    return arc + 1;
}

/**
 * Bit 0: 1 if found, else 0
 * Bit 1: 1 if protected, else 0
 * Bit 2: 1 if archived, else 0
 */
uint8_t detect_prgm(const char *name)
{
    char *fname, buf[9];
    void *pos = NULL;
    uint8_t type;
    while ((fname = ti_DetectAny(&pos, NULL, &type)))
    {
        if (type != TI_PRGM_TYPE && type != TI_PPRGM_TYPE && type != TI_TPRGM_TYPE)
            continue;
        strcpy(buf, fname);
        if (!(buf[0] & 64))
            buf[0] |= 64;
        if (!strcasecmp(name, fname))
        {
            uint8_t res = 0b001;
            if (type == TI_PPRGM_TYPE)
                res = 0b011;
            ti_var_t v = ti_Open(name, "r");
            if (ti_IsArchived(v))
                res |= 0b100;
            ti_Close(v);
            return res;
        }
    }
    return 0;
}

void hide(const char *prgm)
{
    uint8_t r = detect_prgm(prgm);
    if (!r)
        return;
    char buf[9];
    strcpy(buf, prgm);
    buf[0] &= 63;
    if (strcmp(prgm, buf))
        ti_RenameVar(prgm, buf, (r & 0b10) ? TI_PPRGM_TYPE : TI_PRGM_TYPE);
}
void unhide(const char *prgm)
{
    char buf[9];
    strcpy(buf, prgm);
    buf[0] &= 63;
    uint8_t r = detect_prgm(buf);
    if (!r)
        return;
    ti_RenameVar(buf, prgm, (r & 0b10) ? TI_PPRGM_TYPE : TI_PRGM_TYPE);
}

tinystl::string &operator+=(tinystl::string &a, const tinystl::string &b)
{
    a.append(b.c_str(), b.c_str() + b.size());
    return a;
}

#endif /* _UTIL_HPP */