#include <tice.h>
#include <graphx.h>

#include <TINYSTL/vector.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#pragma region macros
#define init()           \
    gfx_Begin();         \
    gfx_SetDrawBuffer(); \
    srandom(rtc_Time())
#define quit() \
    gfx_End()
#pragma endregion macros

struct entity
{
    uint8_t initiative;
    int hp;

    bool operator<(const entity &x)
    {
        return initiative < x.initiative;
    }
};
const char entity_format[] = "[%u] I: %u H: %d";
char buffer[256];

void clearscr()
{
    gfx_FillScreen(255);
    gfx_SetColor(0);
    gfx_Rectangle_NoClip(0, 0, LCD_WIDTH - 1, 12);
    gfx_PrintStringXY("D&D Encounter Tools by David", 2, 2);
}
sk_key_t getkey()
{
    sk_key_t key;
    while (!(key = os_GetCSC()))
        delay(10);
    return key;
}
sk_key_t getkey(int count, ...)
{
    while (true)
    {
        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        va_list args;
        va_start(args, count);
        bool found = false;
        for (uint8_t i = 0; i < count; i++)
        {
            if (key == va_arg(args, int))
            {
                found = true;
                break;
            }
        }
        va_end(args);
        if (found)
            return key;
    }
}
template <typename T>
typename tinystl::vector<T>::iterator lower_bound(typename tinystl::vector<T>::iterator first, typename tinystl::vector<T>::iterator last, const T &value)
{
    typename tinystl::vector<T>::iterator it;
    size_t count, step;
    count = last - first;

    while (count > 0)
    {
        it = first;
        step = count / 2;
        for (size_t i = 0; i < step; i++)
            it++;
        if (*it < value)
        {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}
template <typename T>
void insert(tinystl::vector<T> &list, const T &value)
{
    list.insert(lower_bound(list.begin(), list.end(), value), value);
}
constexpr uint8_t key2int(sk_key_t key)
{
    return key == sk_1
               ? 1
               : (key == sk_2
                      ? 2
                      : (key == sk_3
                             ? 3
                             : (key == sk_4
                                    ? 4
                                    : (key == sk_5
                                           ? 5
                                           : (key == sk_6
                                                  ? 6
                                                  : (key == sk_7
                                                         ? 7
                                                         : (key == sk_8
                                                                ? 8
                                                                : (key == sk_9 ? 9 : 0))))))));
}
int getinput(int start = 0)
{
    gfx_BlitScreen();
    int val = start;
    sk_key_t key;
    gfx_SetTextXY(100, 100);
    gfx_PrintInt(val, 1);
    gfx_SetColor(255);
    gfx_SwapDraw();
    while (true)
    {
        key = getkey(14, sk_0, sk_1, sk_2, sk_3, sk_4, sk_5, sk_6, sk_7, sk_8, sk_9, sk_Clear, sk_Del, sk_Enter, sk_Chs);
        switch (key)
        {
        case sk_Enter:
            return val;
        case sk_Clear:
            val = 0;
            break;
        case sk_Del:
            val /= 10;
            break;
        case sk_Chs:
            val = -val;
            break;
        default:
            val = val * 10 + key2int(key);
            break;
        }
        gfx_FillRectangle_NoClip(100, 100, 50, 8);
        gfx_SetTextXY(100, 100);
        gfx_PrintInt(val, 1);
        gfx_SwapDraw();
    }
}

int8_t page = 0;
tinystl::vector<entity> v;
uint8_t selentity = 0; // This number should be between 0 and 7 (inclusive).
uint8_t selpage = 0;   // This number * 8 + selentity = actual selected.
uint8_t initiative;
bool heal_neg;

void page_main()
{
    clearscr();
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Encounter Tools", 50, 30);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("[1] New encounter", 40, 65);
    gfx_PrintStringXY("[2] Quit", 40, 90);
    gfx_SwapDraw();

    switch (getkey(3, sk_1, sk_2, sk_Clear))
    {
    case sk_Clear:
    case sk_2:
        page = -1;
        return;
    case sk_1:
        page = 1;
        return;
    }
}
void page_new()
{
    v.clear();
    clearscr();
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("New Encounter", 54, 30);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("[+] Add entry", 40, 65);
    gfx_PrintStringXY("[Clear] Exit", 40, 90);
    gfx_PrintStringXY("[0]-[7] Select entry", 40, 115);
    gfx_PrintStringXY("[8] Previous Page", 40, 140);
    gfx_PrintStringXY("[9] Next entry", 40, 165);
    gfx_SwapDraw();

    getkey();
    page = 2;
    return;
}
void page_edit()
{
    clearscr();
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Edit Encounter", 50, 30);
    gfx_SetTextScale(1, 1);
    uint16_t begin = selpage * 8;
    const size_t s = v.size();
    for (uint16_t i = 0, p = begin; i < 8 && p < s; i++, p++)
    {
        const entity &e = v[p];
        sprintf(buffer, entity_format, i, e.initiative, e.hp);
        gfx_PrintStringXY(buffer, 40, 65 + 18 * i);
    }
    gfx_PrintStringXY("[8] Previous Page", 40, 209);
    gfx_PrintStringXY("[9] Next Page", 40, 227);
    gfx_SwapDraw();

    sk_key_t key = getkey(12, sk_0, sk_1, sk_2, sk_3, sk_4, sk_5, sk_6, sk_7, sk_8, sk_9, sk_Clear, sk_Add);
    switch (key)
    {
    case sk_8:
        if (selpage)
            selpage--;
        return;
    case sk_9:
        selpage++;
        if (selpage * 8 >= s)
            selpage--;
        return;
    case sk_Clear:
        page = 0;
        return;
    case sk_Add:
        page = 4;
        return;
    default:
        selentity = key2int(key);
        page = 3;
        return;
    }
}
void page_selected()
{
    clearscr();
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Edit Entity", 88, 30);
    entity &e = v[selpage * 8 + selentity];
    sprintf(buffer, entity_format, selentity, e.initiative, e.hp);
    gfx_PrintStringXY(buffer, (LCD_WIDTH - gfx_GetStringWidth(buffer)) / 2, 50);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("[+] Heal", 40, 85);
    gfx_PrintStringXY("[-] Damage", 40, 110);
    gfx_PrintStringXY("[Clear] Back", 40, 135);
    gfx_SwapDraw();

    sk_key_t key = getkey(3, sk_Add, sk_Sub, sk_Clear);
    switch (key)
    {
    case sk_Clear:
        page = 2;
        return;
    default:
        heal_neg = key == sk_Sub;
        page = 7;
        return;
    }
}
void _page_new_entity_header()
{
    clearscr();
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("New Entity", 66, 30);
    gfx_SetTextScale(1, 1);
}
void page_new_entity1()
{
    _page_new_entity_header();
    gfx_PrintStringXY("Choose initiative method:", 40, 65);
    gfx_PrintStringXY("[1] Roll dice", 40, 90);
    gfx_PrintStringXY("[2] Manual input", 40, 115);
    gfx_SwapDraw();

    sk_key_t key = getkey(3, sk_1, sk_2, sk_Clear);
    switch (key)
    {
    case sk_Clear:
        page = 2;
        return;
    case sk_1:
        page = 8;
        return;
    case sk_2:
        page = 5;
        return;
    }
}
void page_new_entity2()
{
    _page_new_entity_header();
    gfx_PrintStringXY("Enter rolled initiative:", 40, 65);
    gfx_SwapDraw();
    initiative = getinput();

    page = 6;
    return;
}
void page_new_entity3()
{
    _page_new_entity_header();
    gfx_PrintStringXY("Enter current HP:", 40, 65);
    gfx_SetTextXY(100, 100);
    gfx_PrintUInt(0, 1);
    gfx_SwapDraw();
    uint16_t hp = getinput();

    entity e{initiative, hp};
    insert(v, e);

    page = 2;
    return;
}
void page_change_hp()
{
    clearscr();
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Edit Entry HP: ", 20, 30);
    if (heal_neg)
        gfx_PrintString("Damage");
    else
        gfx_PrintString("Heal");
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Enter change of HP:", 40, 65);
    gfx_SwapDraw();
    uint16_t change = getinput();

    entity &e = v[selpage * 8 + selentity];
    if (heal_neg)
        e.hp -= change;
    else
        e.hp += change;
    page = 2;
    return;
}
void page_new_entity4()
{
    _page_new_entity_header();
    gfx_PrintStringXY("Enter DEX modifier:", 40, 65);
    gfx_SwapDraw();
    uint8_t mod = getinput();
    initiative = randInt(1, 20) + mod;
    page = 6;
    return;
}

typedef void (*funcptr)();
funcptr pages[] = {page_main, page_new, page_edit, page_selected, page_new_entity1, page_new_entity2, page_new_entity3, page_change_hp, page_new_entity4};

int main()
{
    init();

    while (page != -1)
        pages[page]();

    quit();
    return 0;
}
