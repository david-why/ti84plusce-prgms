#ifndef _GAMES_HPP
#define _GAMES_HPP

#include "gfxutil.hpp"
#include "nummenu.hpp"
#include "exec.hpp"
#include "cesium.h"
#include "fatutils.h"
#include "TINYSTL/string.h"
#include "TINYSTL/vector.h"
#include <stdio.h>
#include <limits.h>

#define _hs(name, filename)                  \
    const char *name##_prgmname = #name;     \
    const char *name##_filename = #filename; \
    void name##_edit(name##_savefile *);     \
    bool name() { return hs_tmpl(name##_prgmname, name##_filename, (void (*)(void *))name##_edit); }
#define _t(name, filename)                    \
    const char *name##_prgmname = #name;      \
    const char *name##_filename = #filename;  \
    void name##_edit(name##_savefile *);      \
    void name##_templates(name##_savefile *); \
    bool name() { return t_tmpl(name##_prgmname, name##_filename, (void (*)(void *))name##_edit, (void (*)(void *))name##_templates); }
#define _r(name)                             \
    const char *name##_prgmname = #name;     \
    bool name()                              \
    {                                        \
        if (gfxutil_YesNo("Run " #name "?")) \
        {                                    \
            gfx_End();                       \
            runprogram(name##_prgmname);     \
        }                                    \
        return false;                        \
    }

template <typename T>
void edit_highscore(T *score)
{
    gfx_End();
    os_ClrHome();
    print("Current highscore:");
    print(*score);
    print();
    *score = input_int("New highscore?", false);
    gfx_Begin();
    gfx_SetDrawBuffer();
}
template <>
void edit_highscore<uint16_t>(uint16_t *score)
{
    gfx_End();
    os_ClrHome();
    print("Current highscore:");
    print(*score);
    print();
    print("Maximum: 65535");
    print();
    *score = input_int("New highscore?", false);
    gfx_Begin();
    gfx_SetDrawBuffer();
}
template <>
void edit_highscore<uint8_t>(uint8_t *score)
{
    gfx_End();
    os_ClrHome();
    print("Current highscore:");
    print(*score);
    print();
    print("Maximum: 255");
    print();
    *score = input_int("New highscore?", false);
    gfx_Begin();
    gfx_SetDrawBuffer();
}

enum int_type_t
{
    INT8 = 8,
    INT16 = 16,
    INT24 = 24
};
struct edit_data
{
    const char *name;
    void *data;
    int_type_t type;
    int max = INT24_MAX;
};
void edit_templ(edit_data datas[], uint8_t len)
{
    const char *names[len];
    for (uint8_t i = 0; i < len; i++)
        names[i] = datas[i].name;
    auto do_edit = [](edit_data &data)
    {
        gfx_End();
        os_ClrHome();
        print("Editing:");
        print(data.name);
        print();
        print("Current:");
        switch (data.type)
        {
        case INT8:
            print((int)*(uint8_t *)data.data);
            break;
        case INT16:
            print((int)*(uint16_t *)data.data);
            break;
        case INT24:
            print(*(int *)data.data);
            break;
        }
        print();
        int m = data.max;
        if (m == INT24_MAX)
            switch (data.type)
            {
            case INT8:
                m = UINT8_MAX;
                break;
            case INT16:
                m = UINT16_MAX;
                break;
            case INT24:
                m = INT24_MAX;
                break;
            }
        print("Maximum:");
        print(m);
        print();
        int n = input_int("New?", false);
        if (n > m)
        {
            print("Too big!");
            while (!os_GetCSC())
                delay(10);
        }
        else
            switch (data.type)
            {
            case INT8:
                *(uint8_t *)data.data = n;
                break;
            case INT16:
                *(uint16_t *)data.data = n;
                break;
            case INT24:
                *(uint24_t *)data.data = n;
                break;
            }
        gfx_Begin();
        gfx_SetDrawBuffer();
    };
    while (true)
    {
        int16_t choice = nummenu(names, len, "Select field");
        if (choice < 0)
            return;
        do_edit(datas[choice]);
    }
}
bool hs_tmpl(const char *prgmname, const char *filename, void (*edit)(void *data))
{
    int16_t choice = nummenu((const char *[]){"Run game", "Edit save/highscore", "Clear save/highscore", "Hide/Unhide"}, 4, prgmname);
    var_t *data;
    int arc;
    switch (choice)
    {
    case 0:
        gfx_End();
        runprogram(prgmname);
        return true;
    case 1:
        data = os_GetAppVarData(filename, &arc);
        if (data == NULL)
        {
            gfxutil_Error("File not found!");
            return true;
        }
        if (arc)
        {
            to_ram(filename);
            data = os_GetAppVarData(filename, NULL);
        }
        edit(data->data);
        if (arc)
            to_archive(filename);
        return true;
    case 2:
        if (!detect(filename))
        {
            gfxutil_Error("File not found!");
            return true;
        }
        if (gfxutil_YesNo("Are you sure?"))
        {
            ti_Delete(filename);
            gfxutil_Info("Deleted!");
        }
        return true;
    case 3:
        if (gfxutil_YesNo("Hidden?"))
            hide(prgmname);
        else
            unhide(prgmname);
        return true;
    default:
        return false;
    }
}
bool t_tmpl(const char *prgmname, const char *filename, void (*edit)(void *), void (*templates)(void *))
{
    int16_t choice = nummenu((const char *[]){"Run game", "Edit save/highscore", "Clear save/highscore", "View templates"}, 4, prgmname);
    var_t *data;
    int arc;
    switch (choice)
    {
    case 0:
        gfx_End();
        runprogram(prgmname);
        return true;
    case 1:
        data = os_GetAppVarData(filename, &arc);
        if (data == NULL)
        {
            gfxutil_Error("File not found!");
            return true;
        }
        if (arc)
        {
            to_ram(filename);
            data = os_GetAppVarData(filename, NULL);
        }
        edit(data->data);
        if (arc)
            to_archive(filename);
        return true;
    case 2:
        if (!detect(filename))
        {
            gfxutil_Error("File not found!");
            return true;
        }
        if (gfxutil_YesNo("Are you sure?"))
        {
            ti_Delete(filename);
            gfxutil_Info("Deleted!");
        }
        return true;
    case 3:
        data = os_GetAppVarData(filename, &arc);
        if (data == NULL)
        {
            gfxutil_Error("File not found!");
            return true;
        }
        if (arc)
        {
            to_ram(filename);
            data = os_GetAppVarData(filename, NULL);
        }
        templates(data->data);
        if (arc)
            to_archive(filename);
        return true;
    default:
        return false;
    }
}

#pragma pack(push, 1)

#pragma region ACERECON
struct ACERECON_savefile
{
    int games;
    int guards;
    int highscore;
    int deaths;
    int coins;
    int levels;
    bool menu_wrap;
    bool fps_meter;
};
_hs(ACERECON, ACRCNDAT);
void ACERECON_edit(ACERECON_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Games played", &save->games, INT24},
            {"Levels passed", &save->levels, INT24},
            {"Guards eliminated", &save->guards, INT24},
            {"Deaths", &save->deaths, INT24},
            {"Highscore", &save->highscore, INT24},
            {"Current coins", &save->coins, INT24}},
        6);
}
#pragma endregion ACERECON

#pragma region ASPIRINC
struct ASPIRINC_savefile
{
    uint8_t easy;
    uint8_t medium;
    uint8_t hard;
};
_hs(ASPIRINC, asprinHI);
void ASPIRINC_edit(ASPIRINC_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Easy score", &save->easy, INT8},
            {"Medium score", &save->medium, INT8},
            {"Hard score", &save->hard, INT8}},
        3);
    return;
    while (true)
    {
        int16_t choice = nummenu((const char *[]){"Easy score", "Medium score", "Hard score"}, 3, "Choose field");
        if (choice < 0)
            return;
        auto do_edit = [](uint8_t &n, const char *str)
        {
            gfx_End();
            os_ClrHome();
            print("Current:");
            print(n);
            print();
            n = input_int(str, false);
            gfx_Begin();
            gfx_SetDrawBuffer();
        };
        switch (choice)
        {
        case 0:
            do_edit(save->easy, "Easy score?");
            break;
        case 1:
            do_edit(save->medium, "Medium score?");
            break;
        case 2:
            do_edit(save->hard, "Hard score?");
            break;
        }
    }
}
#pragma endregion ASPIRINC

#pragma region CALCUZAP
struct CALCUZAP_savefile
{
    struct entry
    {
        char name[26];
        char score[6];
        unsigned int get_score() const
        {
            unsigned int sc = 0;
            for (uint8_t i = 0; i < 6; i++)
                sc = sc * 10 + score[i] - '0';
            return sc;
        }
    } entries[12];
};
_t(CALCUZAP, CZapHigh);
void CALCUZAP_edit(CALCUZAP_savefile *save)
{
    char buf[12][34];
    const char *names[12];
    while (true)
    {
        for (uint8_t i = 0; i < 12; i++)
        {
            strncpy(buf[i], save->entries[i].name, 26);
            buf[i][26] = ' ';
            strncpy(buf[i] + 27, save->entries[i].score, 6);
            buf[i][33] = 0;
            names[i] = buf[i];
        }
        int16_t choice = nummenu(names, 12, "Select entry:");
        if (choice < 0)
            return;
        gfx_End();
        os_ClrHome();
        os_GetStringInput((char *)"Name?", buf[0], 27);
        uint8_t len = strlen(buf[0]);
        strncpy(save->entries[choice].name, buf[0], 26);
        memset(save->entries[choice].name + len, ' ', 26 - len);
        print();
        os_GetStringInput((char *)"Score?", buf[0], 7);
        strncpy(save->entries[choice].score, buf[0], 6);
        gfx_Begin();
        gfx_SetDrawBuffer();
    }
}
void CALCUZAP_templates(CALCUZAP_savefile *save)
{
    int16_t choice = nummenu((const char *[]){"Clear highscore (with spaces)", "Set all highscore to one name+score"}, 2, "CALCUZAP templates");
    char buf[27];
    uint8_t len;
    switch (choice)
    {
    case 0:
        memset(save, ' ', sizeof(CALCUZAP_savefile));
        gfxutil_Info("Done!");
        return;
    case 1:
        gfx_End();
        os_ClrHome();
        os_GetStringInput((char *)"Name?", buf, 27);
        len = strlen(buf);
        memset(buf + len, ' ', 26 - len);
        print();
        for (uint8_t i = 0; i < 12; i++)
            memcpy(save->entries[i].name, buf, 26);
        os_GetStringInput((char *)"Score?", buf, 7);
        len = strlen(buf);
        memmove(buf + 6 - len, buf, len);
        memset(buf, '0', 6 - len);
        for (uint8_t i = 0; i < 12; i++)
            memcpy(save->entries[i].score, buf, 6);
        gfx_Begin();
        gfx_SetDrawBuffer();
        gfxutil_Info("Done!");
        return;
    default:
        return;
    }
}
#pragma endregion CALCUZAP

#pragma region CE2048
struct CE2048_savefile
{
    uint8_t unknown[48];
    int score, highscore;
};
_hs(CE2048, CE2048);
void CE2048_edit(CE2048_savefile *save)
{
    edit_templ((edit_data[]){{"Current score", &save->score, INT24}, {"Highscore", &save->highscore, INT24}}, 2);
    return;
    while (true)
    {
        int16_t choice = nummenu((const char *[]){"Current score", "Highscore"}, 2, "Choose field");
        if (choice < 0)
            return;
        auto do_edit = [](int &n, const char *str)
        {
            gfx_End();
            os_ClrHome();
            print("Current:");
            print(n);
            print();
            n = input_int(str, false);
            gfx_Begin();
            gfx_SetDrawBuffer();
        };
        switch (choice)
        {
        case 0:
            do_edit(save->score, "Score?");
            break;
        case 1:
            do_edit(save->highscore, "Highscore?");
            break;
        }
    }
}
#pragma endregion CE2048

#pragma region DINO
typedef int DINO_savefile;
_hs(DINO, DinoHS);
void DINO_edit(int *score)
{
    edit_highscore(score);
}
#pragma endregion DINO

#pragma region FALLDOWN
struct FALLDOWN_savefile
{
    uint8_t unknown;
    int slow;
    int medium;
    int fast;
    int hyper;
    uint8_t speed;
};
_hs(FALLDOWN, FALLDDAT);
void FALLDOWN_edit(FALLDOWN_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Slow score", &save->slow, INT24},
            {"Medium score", &save->medium, INT24},
            {"Fast score", &save->fast, INT24},
            {"Hyper score", &save->hyper, INT24}},
        4);
    return;
    while (true)
    {
        int16_t choice = nummenu((const char *[]){"Slow score", "Medium score", "Fast score", "Hyper score"}, 4, "Choose field");
        if (choice < 0)
            return;
        auto do_edit = [](int &n, const char *str)
        {
            gfx_End();
            os_ClrHome();
            print("Current:");
            print(n);
            print();
            n = input_int(str, false);
            gfx_Begin();
            gfx_SetDrawBuffer();
        };
        switch (choice)
        {
        case 0:
            do_edit(save->slow, "Slow score?");
            break;
        case 1:
            do_edit(save->medium, "Medium score?");
            break;
        case 2:
            do_edit(save->fast, "Fast score?");
            break;
        case 3:
            do_edit(save->hyper, "Hyper score?");
            break;
        }
    }
}
#pragma endregion FALLDOWN

#pragma region FLAPPYB
typedef int FLAPPYB_savefile;
_hs(FLAPPYB, FLAPPYB);
void FLAPPYB_edit(int *save) { edit_highscore(save); }
#pragma endregion FLAPPYB

#pragma region HAILSTRM
struct HAILSTRM_savefile
{
    uint24_t highscore;
    uint8_t unknown;
    uint16_t games;
    uint24_t asteroids;
    uint24_t shots;
    uint8_t shield_color;
    uint8_t bullet_color;
    bool menu_wrap;
    bool show_fps;
    bool hard_mode;
    bool star_field_off;
    uint24_t time;
    uint8_t difficulty;
};
_hs(HAILSTRM, HSTRMDAT);
void HAILSTRM_edit(HAILSTRM_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Highscore", &save->highscore, INT24},
            {"Games played", &save->games, INT16},
            {"Asteroids destroyed", &save->asteroids, INT24},
            {"Shots fired", &save->shots, INT24},
            {"Time played", &save->time, INT24}},
        5);
    return;
    while (true)
    {
        int16_t choice = nummenu((const char *[]){"Highscore", "Games played", "Asteroids destroyed", "Shots fired", "Time played"}, 5, "Choose field");
        if (choice < 0)
            return;
        auto do_edit = [](auto &n, const char *str)
        {
            gfx_End();
            os_ClrHome();
            print("Current:");
            print((int)n);
            print();
            n = input_int(str, false);
            gfx_Begin();
            gfx_SetDrawBuffer();
        };
        switch (choice)
        {
        case 0:
            do_edit(save->highscore, "Highscore?");
            break;
        case 1:
            do_edit(save->games, "Games played?");
            break;
        case 2:
            do_edit(save->asteroids, "Asteroids?");
            break;
        case 3:
            do_edit(save->shots, "Shots?");
            break;
        case 4:
            do_edit(save->time, "Time played?");
            break;
        }
    }
}
#pragma endregion HAILSTRM

#pragma region NOTOUCH
typedef uint8_t NOTOUCH_savefile;
_hs(NOTOUCH, clrhs);
void NOTOUCH_edit(uint8_t *score) { edit_highscore(score); }
#pragma endregion NOTOUCH

#pragma region PJUMP
struct PJUMP_savefile
{
    int24_t highscore;
    int24_t runs;
    int24_t quits;
    int24_t deaths;
};
_hs(PJUMP, PlJmpDat);
void PJUMP_edit(PJUMP_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Highscore", &save->highscore, INT24},
            {"Deaths", &save->deaths, INT24},
            {"Quits", &save->quits, INT24},
            {"Runs", &save->runs, INT24},
        },
        4);
    return;
    while (true)
    {
        int16_t choice = nummenu((const char *[]){"Highscore", "Deaths", "Quits", "Runs"}, 4, "Choose field");
        if (choice < 0)
            return;
        auto do_edit = [](int24_t &n, const char *str)
        {
            gfx_End();
            os_ClrHome();
            print("Current:");
            print(n);
            print();
            n = input_int(str, false);
            gfx_Begin();
            gfx_SetDrawBuffer();
        };
        switch (choice)
        {
        case 0:
            do_edit(save->highscore, "Highscore?");
            break;
        case 1:
            do_edit(save->deaths, "Deaths?");
            break;
        case 2:
            do_edit(save->quits, "Quits?");
            break;
        case 3:
            do_edit(save->runs, "Runs?");
            break;
        }
    }
}
#pragma endregion PJUMP

#pragma region PORTAL
/* All numbers are 1-based. */
struct PORTAL_savefile
{
    uint8_t orig;
    uint8_t color;
    uint8_t pack1;
    uint8_t pack2;
    uint8_t pack3;
    uint8_t unknown[2];
};
_hs(PORTAL, PORTALKP);
struct PORTAL_packfile
{
    uint8_t levels;
    uint8_t data[0];
};
const char *PORTAL_pack1 = "PORTAL1P", *PORTAL_pack2 = "PORTAL2P", *PORTAL_pack3 = "PORTAL3P";
#define PORTAL_packlevels(n) __extension__(                \
    {                                                      \
        var_t *v = os_GetAppVarData(PORTAL_pack##n, NULL); \
        int ans = 0;                                       \
        if (v != NULL)                                     \
            ans = ((PORTAL_packfile *)v->data)->levels;    \
        ans;                                               \
    })
void PORTAL_edit(PORTAL_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Built-in levels", &save->orig, INT8, 41},
            {"Pack 1 levels", &save->pack1, INT8, PORTAL_packlevels(1)},
            {"Pack 2 levels", &save->pack2, INT8, PORTAL_packlevels(2)},
            {"Pack 3 levels", &save->pack3, INT8, PORTAL_packlevels(3)}},
        3);
    return;
    int lvlb = 40, lvl1 = PORTAL_packlevels(1), lvl2 = PORTAL_packlevels(2), lvl3 = PORTAL_packlevels(3);
    while (true)
    {
        int16_t choice = nummenu((const char *[]){"Built-in levels", "Pack 1 levels", "Pack 2 levels", "Pack 3 levels"}, 4, "Choose field");
        if (choice < 0)
            return;
        auto do_edit = [](uint8_t &n, int max, const char *str)
        {
            gfx_End();
            os_ClrHome();
            print("Maximum:");
            print(max);
            print();
            print("Current:");
            print(n - 1);
            print();
            int num = input_int(str, false);
            if (num > max)
            {
                print("Too big!");
                while (!os_GetCSC())
                    delay(10);
            }
            else if (num < 0)
            {
                print("Too small!");
                while (!os_GetCSC())
                    delay(10);
            }
            else
            {
                n = num + 1;
            }
            gfx_Begin();
            gfx_SetDrawBuffer();
        };
        switch (choice)
        {
        case 0:
            do_edit(save->orig, lvlb, "Score?");
            break;
        case 1:
            do_edit(save->pack1, lvl1, "Pack 1?");
            break;
        case 2:
            do_edit(save->pack2, lvl2, "Pack 2?");
            break;
        case 3:
            do_edit(save->pack3, lvl3, "Pack 3?");
            break;
        }
    }
}
#pragma endregion PORTAL

#pragma region RGBE
struct RGBE_savefile
{
    uint8_t header[10]; // 'RGBE\xae\x01\x00\x00\x00\x00'
    uint16_t level_stuck_0;
    uint16_t level_stuck_1;
    uint16_t level_stuck_2;
    uint8_t unknown[168];
};
_hs(RGBE, RGBESAVE);
void RGBE_edit(RGBE_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Level stuck 1", &save->level_stuck_0, INT16, 400},
            {"Level stuck 2", &save->level_stuck_1, INT16, 400},
            {"Level stuck 3", &save->level_stuck_2, INT16, 400}},
        3);
}
#pragma endregion RGBE

#pragma region SNAKECE
typedef int SNAKECE_savefile;
_hs(SNAKECE, SnakeCE);
void SNAKECE_edit(int *score) { edit_highscore(score); }
#pragma endregion SNAKECE

#pragma region SPLAT
struct SPLAT_savefile
{
    uint8_t unknown1[4];
    uint24_t classic;
    uint24_t hardcore;
    uint24_t infinity;
    uint8_t unknown2;
};
_hs(SPLAT, SplatHs);
void SPLAT_edit(SPLAT_savefile *save)
{
    edit_templ(
        (edit_data[]){
            {"Classic highscore", &save->classic, INT24},
            {"Hardcore highscore", &save->hardcore, INT24},
            {"Infinity highscore", &save->infinity, INT24}},
        3);
}
#pragma endregion SPLAT

#pragma region STARCOL
typedef char STARCOL_savefile;
_hs(STARCOL, StColDat);
void STARCOL_edit(STARCOL_savefile *save)
{
    real_t t = os_StrToReal(save, NULL);
    int score = os_RealToInt24(&t);
    edit_highscore(&score);
    char buf[9];
    t = os_Int24ToReal(score);
    os_RealToStr(buf, &t, 0, 0, 0);
    uint8_t len = strlen(buf) + 1;
    ti_Delete(STARCOL_filename);
    ti_var_t var = ti_Open(STARCOL_filename, "w");
    ti_Resize(len, var);
    ti_Write(buf, len, 1, var);
    ti_Close(var);
}
#pragma endregion STARCOL

#pragma region TAXI
typedef uint16_t TAXI_savefile;
_hs(TAXI, Taxi);
void TAXI_edit(uint16_t *score) { edit_highscore(score); }
#pragma endregion TAXI

_r(ADPTPRKR);
_r(ADVENTUR);
_r(ADVNTURE);
_r(ANDROIDE);
_r(BOXMAN);
_r(CHESS);
_r(CMONSTER); // TODO
_r(CRYSTANN);
_r(DSTARCE);

#pragma pack(pop)

#undef _hs
#undef _r

#pragma region utils
tinystl::string vector2path(const tinystl::vector<tinystl::string> &v)
{
    if (v.size() == 0)
        return "/";
    tinystl::string s;
    for (auto &x : v)
    {
        s += "/";
        s += x;
    }
    return s;
}
struct ti_8x_file
{
    char sig[11];
    char comment[42];
    uint16_t datasize;
    uint8_t data[1];
};
struct ti_8x_var
{
    uint16_t unknown;
    uint16_t length;
    uint8_t type;
    char name[8];
    uint8_t version;
    uint8_t flag;
    uint16_t length2; // copy of length
    uint8_t data[1];
};
void getprog()
{
    gfx_End();
    os_ClrHome();
    os_PutStrFull("Waiting for USB device...");
    if (fatutil_Init(10))
    {
        os_NewLine();
        os_PutStrFull("Init FAT failed");
        getkey;
        fatutil_Deinit();
        gfx_Begin();
        gfx_SetDrawBuffer();
        return;
    }

    const char *actions[] = {"Change directory", "Send file from current directory"};
    tinystl::vector<tinystl::string> path;

    char buf[20];
    gfx_Begin();
    gfx_SetDrawBuffer();
    while (true)
    {
        int16_t choice = nummenu(actions, 2, vector2path(path).c_str());
        if (choice == 0)
        {
            tinystl::vector<fatutil_entry_t> v = fatutil_ListDir(vector2path(path).c_str(), FAT_LIST_DIRONLY);
            int16_t choice = nummenu((const char *(*)(fatutil_entry_t &))[](fatutil_entry_t & s) { return (const char *)s.entry.filename; }, v, v.size());
            if (choice >= 0)
            {
                char *fn = v[choice].entry.filename;
                if (!strcmp(fn, ".."))
                    path.pop_back();
                else if (strcmp(fn, "."))
                    path.push_back(v[choice].entry.filename);
            }
        }
        else if (choice == 1)
        {
            tinystl::vector<fatutil_entry_t> v = fatutil_ListDir(vector2path(path).c_str(), FAT_LIST_FILEONLY);
            for (auto it = v.begin(); it != v.end();)
            {
                char *name = it->entry.filename;
                char *dot = strchr(name, '.');
                if (dot == NULL || (strcasecmp(dot + 1, "8xp") && strcasecmp(dot + 1, "8xv")))
                    v.erase(it++);
                else
                    it++;
            }
            int16_t choice = nummenu((const char *(*)(fatutil_entry_t &))[](fatutil_entry_t & s) { return (const char *)s.entry.filename; }, v, v.size());
            if (choice >= 0)
            {
                fatutil_entry_t &entry = v[choice];
                fatutil_file_t *file = fatutil_Open(entry);
                if (file == NULL)
                {
                    gfxutil_Error("Cannot open file!");
                    break;
                }
                uint32_t size = fatutil_GetSize(file);
                ti_8x_file *buffer = (ti_8x_file *)malloc(size);
                if (buffer == NULL)
                {
                    gfxutil_Error("Cannot malloc!");
                    break;
                }
                fatutil_Read(file, size, buffer);
                fatutil_Close(file);
                ti_8x_var *var = (ti_8x_var *)buffer->data;
                if (var->type == TI_PRGM_TYPE || var->type == TI_PPRGM_TYPE || var->type == TI_APPVAR_TYPE)
                {
                    strncpy(buf, var->name, 8);
                    buf[8] = 0;
                    ti_var_t slot = ti_OpenVar(buf, "w", var->type);
                    if (ti_Resize(var->length - 2, slot) <= 0)
                    {
                        gfxutil_Error("ERROR: Cannot resize file!");
                        break;
                    }
                    ti_Write(var->data + 2, var->length - 2, 1, slot);
                    ti_SetArchiveStatus(true, slot);
                    ti_Close(slot);
                    gfxutil_Info("Transferred file!");
                }
                else
                {
                    sprintf(buf, "Unknown %u!", var->type);
                    gfxutil_Info(buf);
                }
                free(buffer);
            }
        }
        else
            break;
    }

    fatutil_Deinit();
    return;
}

void install_cesium()
{
    ti_var_t var = ti_OpenVar("CESIUM", "w", TI_PPRGM_TYPE);
    if (!var)
    {
        gfxutil_Error("Cannot write CESIUM!");
        return;
    }
    if (ti_Resize(sizeof(cesium_data), var) <= 0)
    {
        gfxutil_Error("Cannot write CESIUM!");
        return;
    }
    ti_Write(cesium_data, sizeof(cesium_data), 1, var);
    ti_SetArchiveStatus(true, var);
    ti_Close(var);
    gfxutil_Info("Installing");
    runprogram("CESIUM");
}

bool utils()
{
    int16_t choice = nummenu((const char *[]){"Install Cesium v3.5.0", "Get vars from USB"}, 2, "Select util");
    switch (choice)
    {
    case 0:
        install_cesium();
        break;
    case 1:
        getprog();
        break;
    default:
        return false;
    }
    return true;
}
#pragma endregion utils

struct game_t
{
    const char *name;
    bool (*action)();
} games[] =
#define g(name)     \
    {               \
#name, name \
    }
    {
        {"Utils", utils},

        // has edit
        g(ACERECON),
        g(ASPIRINC),
        g(CALCUZAP),
        g(CE2048),
        g(DINO),
        g(FALLDOWN),
        g(FLAPPYB),
        g(HAILSTRM),
        g(NOTOUCH),
        g(PJUMP),
        g(PORTAL),
        g(RGBE),
        g(SNAKECE),
        g(SPLAT),
        g(STARCOL),
        g(TAXI),

        // just run
        g(ADPTPRKR),
        g(ADVENTUR),
        g(ADVNTURE),
        g(ANDROIDE),
        g(BOXMAN),
        g(CHESS),
        g(CMONSTER),
        g(CRYSTANN),
        g(DSTARCE)};
#undef g
constexpr uint8_t NGAMES = sizeof(games) / sizeof(game_t);

#endif /* _GAMES_HPP */