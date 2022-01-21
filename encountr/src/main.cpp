#include <tice.h>
#include <fileioc.h>
#include <graphx.h>

#include <stdlib.h>
#include <string.h>

uint8_t sel = 0, n = 0;
char buf[128];

#pragma pack(push, 1)
struct entry
{
    char *name;
    int16_t hp;
    int8_t initmod;
    uint8_t initroll;
} **items = (entry **)malloc(0);
#pragma pack(pop)

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
        gfx_SetColor(16);
    else
        gfx_SetColor(224);
    gfx_Rectangle_NoClip(10, y, 300, 50);
    gfx_SetTextFGColor(0);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY(items[i]->name, 20, y + 10);
    gfx_SetTextFGColor(39);
    gfx_PrintStringXY("HP: ", 20, y + 30);
    gfx_PrintInt(items[i]->hp, 1);
    gfx_SetTextFGColor(16);
    gfx_PrintStringXY("Init.: ", 20, y + 40);
    gfx_PrintUInt(items[i]->initroll, 1);
    gfx_PrintChar('+');
    gfx_PrintInt(items[i]->initmod, 1);
    gfx_PrintChar('=');
    gfx_PrintInt(items[i]->initroll + items[i]->initmod, 1);
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
    qsort(items, n, sizeof(entry *), [](const void *a, const void *b)
          {
              const entry *x = (const entry *)a, *y = (const entry *)b;
              return ((int)y->initmod + y->initroll - x->initmod - x->initroll) * 2 + (y->initroll > x->initroll ? 1 : -1);
          });
}

void edit(uint8_t i, bool create = false, bool forceroll = false)
{
    if (i >= n)
        return;
    if (create)
    {
        gfx_End();
        os_ClrHome();
        os_GetStringInput((char *)"Name:", buf, 128);
        items[i]->name = (char *)malloc(strlen(buf) + 1);
        strcpy(items[i]->name, buf);
    }
    boot_ClearVRAM();
    gfx_SetTextFGColor(0);
    disptitle("HP: ");
    items[i]->hp = getinput(items[i]->hp);
    boot_ClearVRAM();
    disptitle("Init. Mod:");
    items[i]->initmod = getinput(items[i]->initmod);
    boot_ClearVRAM();
    if (!forceroll)
    {
        disptitle("Roll initiative?");
        gfx_PrintStringXY("Press [=] to roll, others to enter", 10, 70);
        sk_key_t k = getkey();
        forceroll = k == sk_Enter;
    }
    if (forceroll)
        items[i]->initroll = randInt(1, 20);
    else
        items[i]->initroll = getinput(items[i]->initroll);
    dosort();
}

void create()
{
    boot_ClearVRAM();
    disptitle("Copies: ");
    uint8_t dup = getinput<uint8_t>(1);
    if (!dup)
        return;
    bool force = false;
    if (dup != 1)
    {
        boot_ClearVRAM();
        disptitle("Roll initiative for each?");
        gfx_PrintStringXY("Press [=] to roll, others to use same", 10, 70);
        sk_key_t k = getkey();
        force = k == sk_Enter;
    }
    realloc(items, sizeof(entry *) * ++n);
    items[n - 1] = (entry *)malloc(sizeof(entry));
    edit(n - 1, true, force);
    for (uint8_t i = 1; i < dup; i++)
    {
        realloc(items, sizeof(entry *) * ++n);
        items[n - 1] = (entry *)malloc(sizeof(entry));
        memcpy(items[n - 1], items[n - 2], sizeof(entry));
        items[n - 1]->name = (char *)malloc(strlen(items[n - 2]->name) + 1);
        strcpy(items[n - 1]->name, items[n - 2]->name);
    }
    dosort();
}

int main()
{
    gfx_Begin();
    while (true)
    {
        boot_ClearVRAM();
        for (uint8_t i = 0; i < 3; i++)
            draw(sel + i, 10 + i * 60);
        sk_key_t k = getkey();
        if (k == sk_Clear)
            break;
        switch (k)
        {
        case sk_Enter:
            edit(sel);
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
        }
    }
    gfx_End();
    for (uint8_t i = 0; i < n; i++)
    {
        free(items[i]->name);
        free(items[i]);
    }
    free(items);
    return 0;
}
