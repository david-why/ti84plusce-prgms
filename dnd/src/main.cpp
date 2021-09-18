#include <stdio.h>
#include <tice.h>
#include <graphx.h>
#include <fileioc.h>
#include <string.h>

#include "gfx/gfx.h"

char buffer[256];
uint8_t mode = 0;
bool quitprgm = false;

void csstring(unsigned int y, uint8_t scale, const char *str, bool box = false)
{
    gfx_SetTextScale(scale, scale);
    unsigned int w = gfx_GetStringWidth(str);
    unsigned int x = (LCD_WIDTH - w) / 2;
    gfx_PrintStringXY(str, x, y);
    if (box)
        gfx_Rectangle(x - 7, y - 7, w + 14, 8 * scale + 14);
    gfx_SetTextScale(1, 1);
}

uint8_t parseint(sk_key_t key)
{
    switch (key)
    {
    case sk_1:
        return 1;
    case sk_2:
        return 2;
    case sk_3:
        return 3;
    case sk_4:
        return 4;
    case sk_5:
        return 5;
    case sk_6:
        return 6;
    case sk_7:
        return 7;
    case sk_8:
        return 8;
    case sk_9:
        return 9;
    default:
        return 0;
    }
}

struct character
{
    struct stats
    {
        uint8_t str, dex, con, inte, wis, cha, prof_bonus;
        bool prof[6];

        stats(uint8_t s, uint8_t d, uint8_t c, uint8_t i, uint8_t w, uint8_t ch, uint8_t p)
            : str(s), dex(d), con(c), inte(i), wis(w), cha(ch), prof_bonus(p)
        {
            memset(prof, false, sizeof(prof));
        }
    };

    stats s;
    bool skills[18];

    character() : s({10, 10, 10, 10, 10, 10, 2})
    {
        memset(skills, false, sizeof(skills));
    }
};

int roll(int num, int sides, int mod, bool adv = false, bool disadv = false)
{
    int ans = mod;
    for (int i = 0; i < num; i++)
    {
        int roll = randInt(1, sides);
        if (adv)
        {
            int r2 = randInt(1, sides);
            if (r2 > roll)
                roll = r2;
        }
        else if (disadv)
        {
            int r2 = randInt(1, sides);
            if (r2 < roll)
                roll = r2;
        }
        ans += roll;
    }
    return ans;
}

int tomod(int score)
{
    if (score < 0)
        score--;
    return (score - 10) / 2;
}

bool detectvar(const char *name)
{
    ti_CloseAll();
    ti_var_t slot = ti_Open(name, "r+");
    if (ti_GetSize(slot) == (uint16_t)(-1))
    {
        ti_Close(slot);
        ti_Delete(name);
        return false;
    }
    ti_Close(slot);
    return true;
}

character *load_character()
{
    ti_CloseAll();
    ti_var_t slot = ti_Open("DNDSAVE", "r");
    character *c = new character;
    ti_Read(c, sizeof(character), 1, slot);
    ti_Close(slot);
    return c;
}

void save_character(const character *c)
{
    ti_CloseAll();
    ti_var_t slot = ti_Open("DNDSAVE", "w");
    ti_Write(c, sizeof(character), 1, slot);
    ti_SetArchiveStatus(true, slot);
    ti_Close(slot);
}

void clearscr()
{
    gfx_FillScreen(255);
    gfx_Rectangle_NoClip(1, 1, LCD_WIDTH - 2, 12);
    const char *mainstr = "DnD tools by David";
    const char *eeql = "[Enter]=toggle [2nd]=save [Clear]=quit";
    const char *ladv = "[<-]=adv. [->]=disadv. [Enter]=roll [Clear]=quit";
    switch (mode)
    {
    case 0:
        sprintf(buffer, "%s", eeql);
        break;
    case 1:
        sprintf(buffer, "%s", eeql);
        break;
    case 2:
        sprintf(buffer, "[Enter]=select [2nd]=roll [Clear]=quit");
        break;
    case 3:
    case 4:
        sprintf(buffer, "%s", ladv);
        break;
    case 5:
        sprintf(buffer, "[y=]=YES [graph]=NO");
        break;
    case 254:
        sprintf(buffer, "[Enter]/[2nd]=roll Any=quit");
        break;
    default:
        sprintf(buffer, "%s", mainstr);
        break;
    }
    gfx_PrintStringXY(buffer, 3, 3);
}

sk_key_t doroll(int num, int sides, int mod, bool adv = false, bool disadv = false)
{
    int r = roll(num, sides, mod, adv, disadv);
    clearscr();
    if (adv)
        csstring(50, 2, "[A] You rolled:");
    else if (disadv)
        csstring(50, 2, "[D] You rolled:");
    else
        csstring(50, 2, "You rolled:");
    if (mod < 0)
        sprintf(buffer, "%dd%d-%d", num, sides, -mod);
    else
        sprintf(buffer, "%dd%d+%d", num, sides, mod);
    csstring(80, 1, buffer);
    if (mod < 0)
        sprintf(buffer, "%d-%d=%d", r - mod, -mod, r);
    else
        sprintf(buffer, "%d+%d=%d", r - mod, mod, r);
    gfx_SetTextFGColor(224);
    csstring(100, 2, buffer, true);
    gfx_SetTextFGColor(0);
    gfx_SwapDraw();

    sk_key_t key;
    while (!(key = os_GetCSC()))
        delay(10);
    return key;
}

const char *menu_choices[] = {"Set character stats", "Set skills", "Roll saving throw", "Roll ability check", "Roll dice", "DELETE CHARACTER"};
constexpr uint8_t n_menu_choices = sizeof(menu_choices) / sizeof(menu_choices[0]);

int8_t menu()
{
    if (quitprgm)
        return -1;
    sk_key_t key;
    uint8_t choice = mode;
    mode = -1;
    while (true)
    {
        clearscr();
        csstring(50, 2, "D&D Tools");
        for (uint8_t i = 0; i < n_menu_choices; i++)
            csstring(85 + i * 20, 1, menu_choices[i], i == choice);
        gfx_Sprite(dnd_icon, 240, 38);
        gfx_SwapDraw();

        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Clear)
            return -1;
        if (key == sk_Enter)
            return choice;
        if (key == sk_Up)
        {
            if (choice == 0)
                choice = n_menu_choices;
            choice--;
        }
        else if (key == sk_Down)
        {
            choice++;
            if (choice == n_menu_choices)
                choice = 0;
        }
    }
}

const char *stat_names[] = {"Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma", "Prof. Bonus"};
#define n_stat_names (7)

void setstats(bool read = true)
{
    character *c;
    if (read)
        c = load_character();
    else
        c = new character;

    uint8_t *stats = (uint8_t *)c + offsetof(character, s);
    uint8_t choice = 0;
    const unsigned int textx = 40;
    const unsigned int boxx = 160;
    const unsigned int boxw = 50;
    while (true)
    {
        clearscr();
        csstring(20, 2, "Set stats");
        for (int i = 0; i < n_stat_names; i++)
        {
            if (i != 6 && c->s.prof[i])
                gfx_SetTextFGColor(224);
            gfx_PrintStringXY(stat_names[i], textx, 50 + 20 * i);
            gfx_SetTextFGColor(0);
            if (i == choice)
                gfx_SetColor(224);
            gfx_Rectangle_NoClip(boxx, 48 + 20 * i, boxw, 12);
            gfx_SetColor(0);
            gfx_SetTextXY(boxx + 2, 50 + 20 * i);
            gfx_PrintUInt(stats[i], 1);
        }
        gfx_SwapDraw();

        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_2nd)
            break;
        if (key == sk_Clear)
        {
            free(c);
            quitprgm = !read;
            return;
        }
        switch (key)
        {
        case sk_Enter:
            if (choice < 6)
                c->s.prof[choice] = !c->s.prof[choice];
            break;
        case sk_Down:
            choice++;
            if (choice == n_stat_names)
                choice = 0;
            break;
        case sk_Up:
            if (choice == 0)
                choice = n_stat_names;
            choice--;
            break;
        case sk_Del:
            stats[choice] /= 10;
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
            stats[choice] = stats[choice] * 10 + parseint(key);
            break;
        }
    }

    save_character(c);
    free(c);
    return;
}
const char *short_names[] = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};
#define n_short_names (6)

void savethrow()
{
    uint8_t choice = 0;
    uint8_t throw_type = 0;
    bool adv = false;
    bool disadv = false;
    while (true)
    {
        clearscr();
        csstring(20, 2, "Saving throw");
        gfx_PrintStringXY("Type:", 20, 55);
        for (uint8_t i = 0; i < n_short_names; i++)
        {
            gfx_PrintStringXY(short_names[i], 70 + 30 * i, 55);
            if (i == throw_type)
            {
                gfx_SetColor(224);
                gfx_Rectangle_NoClip(68 + 30 * i, 53, gfx_GetStringWidth(short_names[i]) + 4, 12);
                gfx_SetColor(0);
            }
            else if (i == choice)
            {
                gfx_Rectangle_NoClip(68 + 30 * i, 53, gfx_GetStringWidth(short_names[i]) + 4, 12);
            }
        }
        gfx_PrintStringXY("Advantage", 20, 80);
        if (adv)
        {
            gfx_SetColor(224);
            gfx_Rectangle_NoClip(18, 78, gfx_GetStringWidth("Advantage") + 4, 12);
            gfx_SetColor(0);
        }
        else if (choice == 6)
        {
            gfx_Rectangle_NoClip(18, 78, gfx_GetStringWidth("Advantage") + 4, 12);
        }
        gfx_PrintStringXY("Disadvantage", 120, 80);
        if (disadv)
        {
            gfx_SetColor(224);
            gfx_Rectangle_NoClip(118, 78, gfx_GetStringWidth("Disadvantage") + 4, 12);
            gfx_SetColor(0);
        }
        else if (choice == 7)
        {
            gfx_Rectangle_NoClip(118, 78, gfx_GetStringWidth("Disadvantage") + 4, 12);
        }
        gfx_SwapDraw();

        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_2nd)
            break;
        switch (key)
        {
        case sk_Clear:
            return;
        case sk_Down:
            if (choice < 6)
                choice = 6;
            else
                choice = 0;
            break;
        case sk_Up:
            if (choice < 6)
                choice = 6;
            else
                choice = 0;
            break;
        case sk_Right:
            if (choice < 6)
            {
                choice++;
                if (choice == 6)
                    choice = 0;
            }
            else
            {
                choice++;
                if (choice == 8)
                    choice = 6;
            }
            break;
        case sk_Left:
            if (choice < 6)
            {
                if (choice == 0)
                    choice = 6;
                choice--;
            }
            else
            {
                if (choice == 6)
                    choice = 8;
                choice--;
            }
            break;
        case sk_Enter:
            if (choice < 6)
                throw_type = choice;
            else if (choice == 6)
            {
                adv = true;
                disadv = false;
            }
            else
            {
                adv = false;
                disadv = true;
            }
            break;
        }
    }
    character *c = load_character();
    int mod = tomod(*((uint8_t *)c + offsetof(character, s) + throw_type));
    if (c->s.prof[throw_type])
        mod += c->s.prof_bonus;

    sk_key_t key = sk_Enter;
    mode = -2;
    while (key == sk_Enter || key == sk_2nd)
        key = doroll(1, 20, mod, adv, disadv);
    mode = 2;
    free(c);
}

const char *skill_names[] = {"Acrobatics",
                             "Animal Handling",
                             "Arcana",
                             "Athletics",
                             "Deception",
                             "History",
                             "Insight",
                             "Intimidation",
                             "Investigation",
                             "Medicine",
                             "Nature",
                             "Perception",
                             "Performance",
                             "Persuation",
                             "Religion",
                             "Sleight of Hand",
                             "Stealth",
                             "Survival"};
const uint8_t skill_scores[] = {1, 4, 3, 0, 5, 3, 4, 5, 3, 4, 3, 4, 5, 5, 3, 1, 1, 4};
#define n_skills (18)

void setskills()
{
    character *c = load_character();

    uint8_t choice = 0;
    constexpr unsigned int textx = 20;
    while (true)
    {
        clearscr();
        csstring(20, 2, "Set skills");
        for (int i = 0; i < n_skills; i++)
        {
            if (c->skills[i])
                gfx_SetTextFGColor(224);
            sprintf(buffer, "%s (%s)", skill_names[i], short_names[skill_scores[i]]);
            gfx_PrintStringXY(buffer, textx, 50 + 10 * i);
            if (c->skills[i])
                gfx_SetTextFGColor(0);
            if (choice == i)
                gfx_Rectangle_NoClip(textx - 2, 49 + 10 * i, LCD_WIDTH - textx * 2 + 4, 10);
        }
        gfx_SwapDraw();

        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Clear)
        {
            free(c);
            return;
        }
        if (key == sk_2nd)
            break;
        switch (key)
        {
        case sk_Enter:
            c->skills[choice] = !c->skills[choice];
            break;
        case sk_Down:
            choice++;
            if (choice == n_skills)
                choice = 0;
            break;
        case sk_Up:
            if (choice == 0)
                choice = n_skills;
            choice--;
            break;
        }
    }

    save_character(c);
    free(c);
}

void skillthrow()
{
    character *c = load_character();

    uint8_t choice = 0;
    bool adv = false;
    bool disadv = false;
    constexpr unsigned int textx = 20;
    while (true)
    {
        clearscr();
        if (adv)
            csstring(20, 2, "[A] Choose skill:");
        else if (disadv)
            csstring(20, 2, "[D] Choose skill:");
        else
            csstring(20, 2, "[N] Choose skill:");
        for (int i = 0; i < n_skills; i++)
        {
            if (c->skills[i])
                gfx_SetTextFGColor(224);
            sprintf(buffer, "%s (%s)", skill_names[i], short_names[skill_scores[i]]);
            gfx_PrintStringXY(buffer, textx, 50 + 10 * i);
            if (c->skills[i])
                gfx_SetTextFGColor(0);
            if (choice == i)
                gfx_Rectangle_NoClip(textx - 2, 49 + 10 * i, LCD_WIDTH - textx * 2 + 4, 10);
        }
        gfx_SwapDraw();

        sk_key_t key;
        while (!(key = os_GetCSC()))
            delay(10);
        if (key == sk_Clear)
        {
            free(c);
            return;
        }
        if (key == sk_Enter)
            break;
        switch (key)
        {
        case sk_Down:
            choice++;
            if (choice == n_skills)
                choice = 0;
            break;
        case sk_Up:
            if (choice == 0)
                choice = n_skills;
            choice--;
            break;
        case sk_Left:
            adv = !adv;
            disadv = false;
            break;
        case sk_Right:
            disadv = !disadv;
            adv = false;
            break;
        }
    }

    int mod = tomod(*((uint8_t *)c + offsetof(character, s) + skill_scores[choice]));
    if (c->skills[choice])
        mod += c->s.prof_bonus;
    free(c);
    sk_key_t key = sk_Enter;
    mode = -2;
    while (key == sk_Enter || key == sk_2nd)
        key = doroll(1, 20, mod, adv, disadv);
    mode = 3;
}

const char *roll_names[] = {"Number", "Sides", "Modifier"};
constexpr uint8_t n_roll_names = sizeof(roll_names) / sizeof(roll_names[0]);

void rollgui()
{
    uint8_t choice = 0;
    int8_t values[n_roll_names] = {1, 20, 0};
    bool adv = false;
    bool disadv = false;
    while (true)
    {
        while (true)
        {
            clearscr();
            if (adv)
                csstring(40, 2, "[A] Roll dice");
            else if (disadv)
                csstring(40, 2, "[D] Roll dice");
            else
                csstring(40, 2, "[N] Roll dice");
            for (int i = 0; i < n_roll_names; i++)
            {
                gfx_PrintStringXY(roll_names[i], 80, 70 + 30 * i);
                if (choice == i)
                    gfx_SetColor(224);
                gfx_Rectangle_NoClip(150, 67 + 30 * i, 100, 14);
                gfx_SetTextXY(153, 70 + 30 * i);
                gfx_PrintInt(values[i], 1);
                gfx_SetColor(0);
            }
            gfx_SwapDraw();

            sk_key_t key;
            while (!(key = os_GetCSC()))
                delay(10);
            if (key == sk_Clear)
                return;
            if (key == sk_Enter)
                break;
            switch (key)
            {
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
                values[choice] = values[choice] * 10 + parseint(key);
                break;
            case sk_Del:
                values[choice] /= 10;
                break;
            case sk_Chs:
                if (choice == 2)
                    values[2] = -values[2];
                break;
            case sk_Left:
                adv = !adv;
                disadv = false;
                break;
            case sk_Right:
                disadv = !disadv;
                adv = false;
                break;
            case sk_Down:
                choice++;
                if (choice == n_roll_names)
                    choice = 0;
                break;
            case sk_Up:
                if (choice == 0)
                    choice = n_roll_names;
                choice--;
                break;
            }
        }

        sk_key_t key = sk_Enter;
        mode = -2;
        while (key == sk_Enter || key == sk_2nd)
            key = doroll(values[0], values[1], values[2], adv, disadv);
        mode = 4;
    }
}

void deletechrtr()
{
    clearscr();
    gfx_SetTextFGColor(224);
    csstring(40, 2, "ARE YOU SURE?");
    csstring(70, 1, "THIS CANNOT BE UNDONE!", true);
    gfx_Rectangle(-1, LCD_HEIGHT - 11, 35, 12);
    gfx_PrintStringXY("YES", 4, LCD_HEIGHT - 9);
    gfx_Rectangle(LCD_WIDTH - 34, LCD_HEIGHT - 11, 35, 12);
    gfx_PrintStringXY("NO", LCD_WIDTH - 24, LCD_HEIGHT - 9);
    gfx_SetTextFGColor(0);
    gfx_SwapDraw();

    sk_key_t key;
    while ((key = os_GetCSC()) != sk_Yequ && key != sk_Graph)
        delay(10);
    if (key == sk_Yequ)
    {
        gfx_FillScreen(255);
        gfx_SetTextFGColor(224);
        csstring(LCD_HEIGHT / 2 - 12, 3, "DELETING...");
        gfx_SwapDraw();

        ti_Delete("DNDSAVE");

        gfx_FillScreen(255);
        csstring(LCD_HEIGHT / 2 - 12, 3, "DELETED!");
        gfx_SetTextFGColor(0);
        gfx_SwapDraw();

        delay(1000);

        mode = 0;
        setstats(false);
    }
}

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    if (!detectvar("DNDSAVE"))
        setstats(false);

    int8_t choice;
    while ((choice = menu()) != -1)
    {
        mode = choice;
        switch (choice)
        {
        case 0:
            setstats();
            break;
        case 1:
            setskills();
            break;
        case 2:
            savethrow();
            break;
        case 3:
            skillthrow();
            break;
        case 4:
            rollgui();
            break;
        case 5:
            deletechrtr();
            break;
        }
    }

    gfx_End();

    return 0;
}
