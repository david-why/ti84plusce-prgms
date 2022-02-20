#include <tice.h>
#include <fileioc.h>
#include <graphx.h>

#include <stdlib.h>
#include <string.h>

uint8_t sel = 0, n = 0, d = 0, cur = 0;
char buf[128];

#pragma pack(push, 1)
struct entry
{
    char name[20];
    int16_t hp;
    int8_t initmod;
    uint8_t initroll;
} items[256];
#pragma pack(pop)

#define setupgfx() gfx_Begin()

void store()
{
    ti_var_t v = ti_Open("ENCOUNTR", "w");
    if (!v)
        os_ThrowError(OS_E_MEMORY);
    ti_Write(items, sizeof(items), 1, v);
    ti_Write(&sel, sizeof(sel), 1, v);
    ti_Write(&n, sizeof(n), 1, v);
    ti_Write(&d, sizeof(d), 1, v);
    ti_Write(&cur, sizeof(cur), 1, v);
    ti_SetArchiveStatus(true, v);
    ti_Close(v);
}

void load()
{
    ti_var_t v = ti_Open("ENCOUNTR", "r");
    if (!v)
        return;
    ti_Read(items, sizeof(items), 1, v);
    ti_Read(&sel, sizeof(sel), 1, v);
    ti_Read(&n, sizeof(n), 1, v);
    ti_Read(&d, sizeof(d), 1, v);
    ti_Read(&cur, sizeof(cur), 1, v);
    ti_Close(v);
}

sk_key_t getkey()
{
    sk_key_t k;
    while (!(k = os_GetCSC()))
        ;
    return k;
}

void draw(uint8_t i, uint8_t y)
{
    if (i >= n)
        return;
    if (i == sel)
        gfx_SetColor(251);
    else
        gfx_SetColor(224);
    if (i == cur)
        gfx_FillRectangle_NoClip(10, y, 300, 70);
    else
        gfx_Rectangle_NoClip(10, y, 300, 70);
    gfx_SetTextFGColor(0);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY(items[i].name, 20, y + 10);
    gfx_SetTextScale(1, 1);
    gfx_SetTextFGColor(39);
    gfx_PrintStringXY("HP: ", 20, y + 30);
    gfx_PrintInt(items[i].hp, 1);
    gfx_SetTextFGColor(16);
    gfx_PrintStringXY("Init.: ", 20, y + 40);
    gfx_PrintUInt(items[i].initroll, 1);
    gfx_PrintChar('+');
    gfx_PrintInt(items[i].initmod, 1);
    gfx_PrintChar('=');
    gfx_PrintInt(items[i].initroll + items[i].initmod, 1);
    gfx_SetTextXY(280, y + 10);
    gfx_PrintChar('#');
    gfx_PrintUInt(i, 1);
}

constexpr uint8_t sk2int(sk_key_t sk)
{
    if (sk == sk_1)
        return 1;
    if (sk == sk_2)
        return 2;
    if (sk == sk_3)
        return 3;
    if (sk == sk_4)
        return 4;
    if (sk == sk_5)
        return 5;
    if (sk == sk_6)
        return 6;
    if (sk == sk_7)
        return 7;
    if (sk == sk_8)
        return 8;
    if (sk == sk_9)
        return 9;
    return 0;
}

template <typename T>
T getinput(T v = 0)
{
    gfx_SetColor(255);
    while (true)
    {
        gfx_FillRectangle_NoClip(10, 70, 300, 8);
        gfx_SetTextXY(10, 70);
        gfx_PrintInt(v, 1);
        sk_key_t k = getkey();
        switch (k)
        {
        case sk_Enter:
            return v;
        case sk_Del:
            v /= 10;
            break;
        case sk_Clear:
            v = 0;
            break;
        case sk_0:
        case sk_1:
        case sk_2:
        case sk_3:
        case sk_4:
        case sk_5:
        case sk_6:
        case sk_7:
        case sk_8:
        case sk_9:
            v = v * 10 + sk2int(k);
            break;
        }
    }
}

void disptitle(const char *s)
{
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY(s, 10, 20);
    gfx_SetTextScale(1, 1);
}

void dosort()
{
    for (uint8_t j = 0; j < n; j++)
        for (uint8_t i = j + 1; i < n; i++)
            if (items[i].initroll + items[i].initmod > items[j].initroll + items[j].initmod || (items[i].initroll + items[i].initmod == items[j].initroll + items[j].initmod && items[i].initroll > items[j].initroll))
            {
                entry e = items[i];
                items[i] = items[j];
                items[j] = e;
            }
}

void edit(uint8_t i, bool create = false, bool forceroll = false)
{
    if (i >= n)
        return;
    if (create)
    {
        gfx_End();
        os_ClrHome();
        os_GetStringInput((char *)"Name:", items[i].name, sizeof(entry::name));
        setupgfx();
    }
    boot_ClearVRAM();
    gfx_SetTextFGColor(0);
    disptitle("HP: ");
    items[i].hp = getinput(items[i].hp);
    boot_ClearVRAM();
    if (create)
    {
        disptitle("Init. Mod:");
        items[i].initmod = getinput(items[i].initmod);
        boot_ClearVRAM();
        if (!forceroll)
        {
            disptitle("Roll init.?");
            gfx_PrintStringXY("[enter] to roll, others to enter", 10, 70);
            sk_key_t k = getkey();
            forceroll = k == sk_Enter;
        }
        if (forceroll)
            items[i].initroll = randInt(1, 20);
        else
            items[i].initroll = getinput(items[i].initroll);
    }
    dosort();
}

void create()
{
    boot_ClearVRAM();
    gfx_SetTextFGColor(0);
    disptitle("Copies: ");
    uint8_t dup = getinput<uint8_t>(1);
    if (!dup)
        return;
    bool force = false, addnum;
    if (dup != 1)
    {
        boot_ClearVRAM();
        disptitle("Roll init. for each?");
        gfx_PrintStringXY("[enter] to roll, others to use same", 10, 70);
        sk_key_t k = getkey();
        force = k == sk_Enter;
        boot_ClearVRAM();
        disptitle("Add num after name?");
        gfx_PrintStringXY("[enter] to add, others to keep name", 10, 70);
        k = getkey();
        addnum = k == sk_Enter;
    }
    memset(&items[n++], 0, sizeof(entry));
    edit(n - 1, true, force);
    gfx_SetTextXY(10, 10);
    for (uint8_t i = 1; i < dup; i++)
    {
        memcpy(&items[n], &items[n - 1], sizeof(entry));
        if (addnum)
            sprintf(items[n].name, "%s%u", items[n - i].name, i);
        else
            strcpy(items[n].name, items[n - 1].name);
        if (force)
            items[n].initroll = randInt(1, 20);
        n++;
    }
    dosort();
}

void delcur()
{
    memmove(items + sel, items + sel + 1, (--n - sel) * sizeof(items[0]));
    if (sel >= n)
        sel = n - 1;
}

int main()
{
    load();
    setupgfx();
    while (true)
    {
        boot_ClearVRAM();
        for (uint8_t i = 0; i < 3; i++)
            draw(d + i, 10 + i * 80);
        sk_key_t k = getkey();
        if (k == sk_Clear)
            break;
        switch (k)
        {
        case sk_Enter:
            edit(sel);
            break;
        case sk_Add:
            create();
            break;
        case sk_Down:
            sel++;
            if (sel >= n)
                sel = 0;
            break;
        case sk_Up:
            if (!n)
                sel = 1;
            else if (sel == 0)
                sel = n;
            sel--;
            break;
        case sk_Left:
            cur++;
            if (cur >= n)
                cur = 0;
            break;
        case sk_Right:
            if (!n)
                cur = 1;
            else if (cur == 0)
                cur = n;
            cur--;
            break;
        case sk_Del:
            delcur();
            break;
        case sk_Vars:
            n = sel = 0;
            break;
        }
        while (d + 2 < sel)
            d++;
        while (d > sel)
            d--;
        store();
    }
    gfx_End();
    return 0;
}
