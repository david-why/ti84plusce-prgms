#ifndef HAS_PRINTF
typedef unsigned int uint24_t;
typedef int int24_t;
#endif

#include <tice.h>
#include <graphx.h>
#include <stdio.h>
#include <string.h>

typedef uint24_t pos_t;

constexpr uint8_t LEFT = (LCD_WIDTH - 18 * 17) / 2,
                  TOP = LCD_HEIGHT / 4,
                  ELEMENTS = 118;

constexpr uint16_t getx(pos_t pos)
{
    return pos & 0xffff;
}

constexpr uint8_t gety(pos_t pos)
{
    return pos >> 16;
}

constexpr pos_t p(uint16_t x, uint8_t y)
{
    return (y << 16) + (x);
}

constexpr pos_t pos_of(uint8_t elem)
{
    if (elem == 1)
        return p(LEFT, TOP);
    if (elem == 2)
        return p(LEFT + 17 * 17, TOP);
    if (elem <= 4)
        return p(LEFT + (elem - 3) * 17, TOP + 16);
    if (elem <= 10)
        return p(LEFT + (elem + 7) * 17, TOP + 16);
    if (elem <= 12)
        return p(LEFT + (elem - 11) * 17, TOP + 32);
    if (elem <= 18)
        return p(LEFT + (elem - 1) * 17, TOP + 32);
    if (elem <= 36)
        return p(LEFT + (elem - 19) * 17, TOP + 48);
    if (elem <= 54)
        return p(LEFT + (elem - 37) * 17, TOP + 64);
    if (elem <= 56)
        return p(LEFT + (elem - 55) * 17, TOP + 80);
    if (elem <= 71)
        return p(LEFT + (elem - 54) * 17, TOP + 120);
    if (elem <= 86)
        return p(LEFT + (elem - 69) * 17, TOP + 80);
    if (elem <= 88)
        return p(LEFT + (elem - 87) * 17, TOP + 96);
    if (elem <= 103)
        return p(LEFT + (elem - 86) * 17, TOP + 136);
    if (elem <= 118)
        return p(LEFT + (elem - 101) * 17, TOP + 96);
    return 0;
}

constexpr uint8_t left(uint8_t pos)
{
    if (pos == 1)
        return 1;
    return pos - 1;
}
constexpr uint8_t right(uint8_t pos)
{
    if (pos == ELEMENTS)
        return ELEMENTS;
    return pos + 1;
}
constexpr uint8_t up(uint8_t pos)
{
    if (pos <= 2)
        return pos;
    if (pos <= 4)
        return 1;
    if (pos <= 10)
        return 2;
    if (pos <= 20)
        return pos - 8;
    if (pos <= 25)
        return 12;
    if (pos <= 30)
        return 13;
    if (pos <= 56)
        return pos - 18;
    if (pos <= 71)
        return 39;
    return pos - 32;
}
constexpr uint8_t down(uint8_t pos)
{
    if (pos == 1)
        return 3;
    if (pos == 2)
        return 10;
    if (pos <= 12)
        return pos + 8;
    if (pos <= 39)
        return pos + 18;
    if (pos <= 86)
        return pos + 32;
    return pos;
}

float atomic_radius[] = {0.79, 0.49, 2.1, 1.4, 1.2, 0.91, 0.75, 0.65, 0.57, 0.51, 2.2, 1.7, 1.8, 1.5, 1.2, 1.1, 0.97, 0.88, 2.8, 2.2, 2.1, 2.0, 1.9, 1.9, 1.8, 1.7, 1.7, 1.6, 1.6, 1.5, 1.8, 1.5, 1.3, 1.2, 1.1, 1.0, 3.0, 2.5, 2.3, 2.2, 2.1, 2.0, 2.0, 1.9, 1.8, 1.8, 1.8, 1.7, 2.0, 1.7, 1.5, 1.4, 1.3, 1.2, 3.3, 2.8, 2.7, 2.7, 2.7, 2.6, 2.6, 2.6, 2.6, 2.5, 2.5, 2.5, 2.5, 2.5, 2.4, 2.4, 2.3, 2.2, 2.1, 2.0, 2.0, 1.9, 1.9, 1.8, 1.8, 1.8, 2.1, 1.8, 1.6, 1.5, 1.4, 1.3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      electronegativity[] = {2.2, -1, 0.98, 1.57, 2.04, 2.55, 3.04, 3.44, 3.98, -1, 0.93, 1.31, 1.61, 1.9, 2.19, 2.58, 3.16, -1, 0.82, 1.0, 1.36, 1.54, 1.63, 1.66, 1.55, 1.83, 1.88, 1.91, 1.9, 1.65, 1.81, 2.01, 2.18, 2.55, 2.96, -1, 0.82, 0.95, 1.22, 1.33, 1.6, 2.16, 1.9, 2.2, 2.28, 2.2, 1.93, 1.69, 1.78, 1.96, 2.05, 2.1, 2.66, -1, 0.79, 0.89, 1.1, 1.12, 1.13, 1.14, 1.13, 1.17, 1.2, 1.2, 1.2, 1.22, 1.23, 1.24, 1.25, 1.1, 1.27, 1.3, 1.5, 2.36, 1.9, 2.2, 2.2, 2.28, 2.54, 2.0, 2.04, 2.33, 2.02, 2.0, 2.2, -1, 0.7, 0.9, 1.1, 1.3, 1.5, 1.38, 1.36, 1.28, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
      ionization_energy[] = {13.5984, 24.5874, 5.3917, 9.3227, 8.298, 11.2603, 14.5341, 13.6181, 17.4228, 21.5645, 5.1391, 7.6462, 5.9858, 8.1517, 10.4867, 10.36, 12.9676, 15.7596, 4.3407, 6.1132, 6.5615, 6.8281, 6.7462, 6.7665, 7.434, 7.9024, 7.881, 7.6398, 7.7264, 9.3942, 5.9993, 7.8994, 9.7886, 9.7524, 11.8138, 13.9996, 4.1771, 5.6949, 6.2173, 6.6339, 6.7589, 7.0924, 7.28, 7.3605, 7.4589, 8.3369, 7.5762, 8.9938, 5.7864, 7.3439, 8.6084, 9.0096, 10.4513, 12.1298, 3.8939, 5.2117, 5.5769, 5.5387, 5.473, 5.525, 5.582, 5.6437, 5.6704, 6.1501, 5.8638, 5.9389, 6.0215, 6.1077, 6.1843, 6.2542, 5.4259, 6.8251, 7.5496, 7.864, 7.8335, 8.4382, 8.967, 8.9587, 9.2255, 10.4375, 6.1082, 7.4167, 7.2856, 8.417, 9.3, 10.7485, 4.0727, 5.2784, 5.17, 6.3067, 5.89, 6.1941, 6.2657, 6.0262, 5.9738, 5.9915, 6.1979, 6.2817, 6.42, 6.5, 6.58, 6.65, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
const char *names[] = {"Hydrogen", "Helium", "Lithium", "Beryllium", "Boron", "Carbon", "Nitrogen", "Oxygen", "Fluorine", "Neon", "Sodium", "Magnesium", "Aluminum", "Silicon", "Phosphorus", "Sulfur", "Chlorine", "Argon", "Potassium", "Calcium", "Scandium", "Titanium", "Vanadium", "Chromium", "Manganese", "Iron", "Cobalt", "Nickel", "Copper", "Zinc", "Gallium", "Germanium", "Arsenic", "Selenium", "Bromine", "Krypton", "Rubidium", "Strontium", "Yttrium", "Zirconium", "Niobium", "Molybdenum", "Technetium", "Ruthenium", "Rhodium", "Palladium", "Silver", "Cadmium", "Indium", "Tin", "Antimony", "Tellurium", "Iodine", "Xenon", "Cesium", "Barium", "Lanthanum", "Cerium", "Praseodymium", "Neodymium", "Promethium", "Samarium", "Europium", "Gadolinium", "Terbium", "Dysprosium", "Holmium", "Erbium", "Thulium", "Ytterbium", "Lutetium", "Hafnium", "Tantalum", "Wolfram", "Rhenium", "Osmium", "Iridium", "Platinum", "Gold", "Mercury", "Thallium", "Lead", "Bismuth", "Polonium", "Astatine", "Radon", "Francium", "Radium", "Actinium", "Thorium", "Protactinium", "Uranium", "Neptunium", "Plutonium", "Americium", "Curium", "Berkelium", "Californium", "Einsteinium", "Fermium", "Mendelevium", "Nobelium", "Lawrencium", "Rutherfordium", "Dubnium", "Seaborgium", "Bohrium", "Hassium", "Meitnerium", "Darmstadtium", "Roentgenium", "Copernicium", "Nihonium", "Flerovium", "Moscovium", "Livermorium", "Tennessine", "Oganesson"},
           *symbols[] = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"};

struct list
{
    float *lst;
    const char *name;
} lists[] = {{atomic_radius, "Atomic Radius"},
             {electronegativity, "Electronegativity"},
             {ionization_energy, "Ionization Energy"}};

uint8_t selected = 1, compkey;
char buffer[512], buffer2[512];
bool comp[ELEMENTS];
uint8_t sortbuf[ELEMENTS], sortbuflen;
real_t r;

void drawtable(bool sel = true, bool cmp = true)
{
    gfx_FillScreen(255);
    const list &compby = lists[compkey];
    sprintf(buffer, "Order by: %s", compby.name);
    gfx_PrintStringXY(buffer, 10, 10);
    for (uint8_t i = 1; i <= ELEMENTS; i++)
    {
        const char *s = symbols[i - 1];
        pos_t p = pos_of(i);
        if (cmp && comp[i - 1])
        {
            uint8_t color = gfx_SetTextFGColor(224);
            gfx_PrintStringXY(s, getx(p), gety(p));
            gfx_SetTextFGColor(color);
        }
        else
        {
            gfx_PrintStringXY(s, getx(p), gety(p));
        }
        if (sel && selected == i)
        {
            gfx_Rectangle(getx(p) - 2, gety(p) - 2, 19, 11);
            uint8_t color = gfx_SetTextFGColor(16);
            sprintf(buffer, "%u. %s", i, names[i - 1]);
            gfx_PrintStringXY(buffer, LEFT + 80, 25);
            gfx_SetTextFGColor(224);
            for (uint8_t j = 0; j < sizeof(lists) / sizeof(lists[0]); j++)
            {
                const list &lst = lists[j];
                if (lst.lst[i - 1] >= 0)
                {
                    r = os_FloatToReal(lst.lst[i - 1]);
                    os_RealToStr(buffer2, &r, 0, 0, -1);
                }
                else
                {
                    strcpy(buffer2, "N/A");
                }
                sprintf(buffer, "%s: %s", lst.name, buffer2);
                gfx_PrintStringXY(buffer, LEFT + 40, 35 + 10 * j);
            }
            gfx_SetTextFGColor(color);
        }
    }
}

void dosort()
{
    const float *lst = lists[compkey].lst;
    for (uint8_t i = 0; i < sortbuflen; i++)
    {
        for (uint8_t j = i + 1; j < sortbuflen; j++)
        {
            if (lst[sortbuf[i]] > lst[sortbuf[j]])
            {
                uint8_t tmp = sortbuf[i];
                sortbuf[i] = sortbuf[j];
                sortbuf[j] = tmp;
            }
        }
    }
}

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(224);

    while (true)
    {
        drawtable();
        gfx_SwapDraw();
        sk_key_t key;
        while (!(key = os_GetCSC()))
            ;
        if (key == sk_Clear)
            break;
        switch (key)
        {
        case sk_Up:
            selected = up(selected);
            break;
        case sk_Down:
            selected = down(selected);
            break;
        case sk_Left:
            selected = left(selected);
            break;
        case sk_Right:
            selected = right(selected);
            break;
        case sk_Enter:
            if (lists[compkey].lst[selected - 1] >= 0)
                comp[selected - 1] = !comp[selected - 1];
            break;
        case sk_2nd:
            gfx_FillScreen(255);
            sortbuflen = 0;
            for (uint8_t i = 0; i < ELEMENTS; i++)
                if (comp[i])
                    sortbuf[sortbuflen++] = i;
            if (sortbuflen == 0)
                break;
            dosort();
            sprintf(buffer, "Sorted %u elements", sortbuflen);
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY(buffer, (LCD_WIDTH - gfx_GetStringWidth(buffer)) / 2, 10);
            gfx_SetTextScale(1, 1);
            sprintf(buffer, "Order by: %s", lists[compkey].name);
            gfx_PrintStringXY(buffer, (LCD_WIDTH - gfx_GetStringWidth(buffer)) / 2, 30);
            r = os_FloatToReal(lists[compkey].lst[sortbuf[sortbuflen - 1]]);
            os_RealToStr(buffer2, &r, 0, 0, -1);
            sprintf(buffer, "Largest: %s (%s)", names[sortbuf[sortbuflen - 1]], buffer2);
            gfx_PrintStringXY(buffer, 20, 60);
            r = os_FloatToReal(lists[compkey].lst[sortbuf[0]]);
            os_RealToStr(buffer2, &r, 0, 0, -1);
            sprintf(buffer, "Smallest: %s (%s)", names[sortbuf[0]], buffer2);
            gfx_PrintStringXY(buffer, 20, 70);
            gfx_PrintStringXY("List:", 20, 100);
            gfx_SetTextXY(20, 120);
            for (uint8_t i = 0; i < sortbuflen; i++)
            {
                gfx_PrintString(symbols[sortbuf[i]]);
                if (i != sortbuflen - 1)
                    gfx_PrintString(", ");
            }
            gfx_SwapDraw();
            while (!os_GetCSC())
                ;
            break;
        case sk_Mode:
            gfx_BlitScreen();
            constexpr uint8_t height = 4 + 16 + 2 + sizeof(lists) / sizeof(lists[0]) * (1 + 8 + 1) + 1 + 2;
            uint16_t swidth = gfx_GetStringWidth("Choose sorting key:") * 2, width = swidth;
            uint8_t space = gfx_GetCharWidth(' ');
            for (uint8_t i = 0; i < sizeof(lists) / sizeof(lists[0]); i++)
            {
                uint8_t w = gfx_GetCharWidth('0' + i + 1) + space + gfx_GetStringWidth(lists[i].name);
                if (w > width)
                    width = w;
            }
            width += 4 + 4;
            uint16_t spleft = (LCD_WIDTH - width) / 2;
            uint16_t sptop = (LCD_HEIGHT - height) / 2;
            uint8_t color = gfx_SetColor(255);
            gfx_FillRectangle(spleft, sptop, width, height);
            gfx_SetColor(color);
            gfx_Rectangle(spleft, sptop, width, height);
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY("Choose sorting key:", (LCD_WIDTH - swidth) / 2, sptop + 4);
            gfx_SetTextScale(1, 1);
            for (uint8_t i = 0; i < sizeof(lists) / sizeof(lists[0]); i++)
            {
                gfx_SetTextXY((LCD_WIDTH - gfx_GetStringWidth(lists[i].name)) / 2, sptop + 4 + 16 + 2 + 1 + i * (1 + 8 + 1));
                gfx_PrintUInt(i + 1, 1);
                gfx_PrintChar(' ');
                gfx_PrintString(lists[i].name);
            }
            gfx_SwapDraw();
            sk_key_t nkey;
            while (!(nkey = os_GetCSC()))
                ;
#define _(n)                                         \
    case sk_##n:                                     \
        if (sizeof(lists) / sizeof(lists[0]) >= (n)) \
            compkey = (n)-1;                         \
        break
            switch (nkey)
            {
                _(1);
                _(2);
                _(3);
                _(4);
                _(5);
                _(6);
                _(7);
                _(8);
                _(9);
            }
#undef _
            const list &lst = lists[compkey];
            for (uint8_t i = 0; i < ELEMENTS; i++)
                if (lst.lst[i] == -1)
                    comp[i] = false;
            break;
        }
    }

    gfx_End();
    return 0;
}
