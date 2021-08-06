#include <graphx.h>
#include <tice.h>
#include <stdio.h>
#include <string.h>

#include "gfx/gfx.h"
#include "gfx_more.h"

#define waitKeyPress() \
    while (!os_GetCSC())
#define getKeyPress(key) \
    while (!(key = os_GetCSC()))

#define WHITE 0
#define BLACK 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define GRAY 5

char str_buffer[100];

/**
 * 0=Number
 * 1=Sides
 * 2=ModifierSign
 * 3=Modifier
 * 4=Advantage
 * 5=Disadvantage
 * 6=ROLL
 */
int selected;

#define selNumber (!selected)
#define selSides (selected == 1)
#define selModSign (selected == 2)
#define selMod (selected == 3)
#define selAdv (selected == 4)
#define selDisadv (selected == 5)
#define selRoll (selected == 6)

bool sign_positive = true;
int advantage;
unsigned int number = 1, sides = 20, modifier;

#define hasDisadvantage (advantage < 0)
#define hasAdvantage (advantage > 0)
#define noAdvantage (!advantage)

bool exited, rollnow;

uint8_t Advantage_data[AdvantageGray_size],
    Disadvantage_data[DisadvantageGray_size];
gfx_sprite_t *Advantage = (gfx_sprite_t *)Advantage_data,
             *Disadvantage = (gfx_sprite_t *)Disadvantage_data;

void drawScreen()
{
    gfx_FillScreen(WHITE);

    gfx_Sprite_NoClip(RollYourDice, 60, 25);

    gfx_Sprite_NoClip(Number, 50, 70);
    gfx_SetColor(BLACK);
    if (selNumber)
        gfx_SetColor(GREEN);
    gfx_Rectangle_NoClip(140, 65, 130, 22);
    sprintf(str_buffer, "%u", number);
    gfx_PrintStringXY(str_buffer, 147, 73);

    gfx_Sprite_NoClip(Sides, 50, 105);
    gfx_SetColor(BLACK);
    if (selSides)
        gfx_SetColor(GREEN);
    gfx_Rectangle_NoClip(140, 100, 130, 22);
    sprintf(str_buffer, "%u", sides);
    gfx_PrintStringXY(str_buffer, 147, 108);

    gfx_Sprite_NoClip(Modifier, 50, 140);
    gfx_SetColor(BLACK);
    if (selModSign)
        gfx_SetColor(GREEN);
    gfx_FillRectangle_NoClip(129, 145, 9, 3);
    if (sign_positive)
    {
        gfx_FillRectangle_NoClip(132, 142, 3, 9);
    }
    gfx_SetColor(BLACK);
    if (selMod)
        gfx_SetColor(GREEN);
    gfx_Rectangle_NoClip(140, 135, 130, 22);
    sprintf(str_buffer, "%u", modifier);
    gfx_PrintStringXY(str_buffer, 147, 143);

    gfx_Sprite_NoClip(AdvantageGray, 55, 170);
    gfx_Sprite_NoClip(DisadvantageGray, 155, 170);
    if (hasAdvantage)
    {
        gfx_Sprite_NoClip(Advantage, 55, 170);
    }
    else if (hasDisadvantage)
    {
        gfx_Sprite_NoClip(Disadvantage, 155, 170);
    }
    gfx_SetColor(GREEN);
    if (selAdv)
    {
        gfx_HorizLine_NoClip(55, 170 + AdvantageGray_height + 3, AdvantageGray_width);
    }
    else if (selDisadv)
    {
        gfx_HorizLine_NoClip(155, 170 + DisadvantageGray_height + 3, DisadvantageGray_width);
    }

    gfx_Sprite_NoClip(ROLL, 140, 200);
    if (selRoll)
    {
        gfx_SetColor(GREEN);
        gfx_Rectangle(140, 200, ROLL_width, ROLL_height);
    }
}

sk_key_t getInput()
{
    sk_key_t key;
    while (!(key = os_GetCSC()))
        ;
    return key;
}

void inputedNumber(uint8_t n)
{
    switch (selected)
    {
    case 0:
        if (number < 1677720)
            number = number * 10 + n;
        break;
    case 1:
        if (sides < 1677720)
            sides = sides * 10 + n;
        break;
    case 3:
        if (modifier < 1677720)
            modifier = modifier * 10 + n;
        break;
    }
}

void parseInput(sk_key_t key)
{
    switch (key)
    {
    case sk_Mode:
        exited = true;
        break;
    case sk_Clear:
        switch (selected)
        {
        case 0:
            number = 0;
            break;
        case 1:
            sides = 0;
            break;
        case 3:
            modifier = 0;
            break;
        case 6:
            exited = true;
            break;
        }
        break;
    case sk_Left:
    case sk_Up:
        selected--;
        if (selected == -1)
            selected = 6;
        break;
    case sk_Down:
    case sk_Right:
        selected++;
        if (selected == 7)
            selected = 0;
        break;
    case sk_1:
        inputedNumber(1);
        break;
    case sk_2:
        inputedNumber(2);
        break;
    case sk_3:
        inputedNumber(3);
        break;
    case sk_4:
        inputedNumber(4);
        break;
    case sk_5:
        inputedNumber(5);
        break;
    case sk_6:
        inputedNumber(6);
        break;
    case sk_7:
        inputedNumber(7);
        break;
    case sk_8:
        inputedNumber(8);
        break;
    case sk_9:
        inputedNumber(9);
        break;
    case sk_0:
        inputedNumber(0);
        break;
    case sk_Del:
        switch (selected)
        {
        case 0:
            number /= 10;
            break;
        case 1:
            sides /= 10;
            break;
        case 3:
            modifier /= 10;
            break;
        }
        break;
    case sk_Enter:
    case sk_2nd:
        switch (selected)
        {
        case 0:
            selected = 1;
            break;
        case 1:
            selected = 3;
            break;
        case 2:
            sign_positive = !sign_positive;
            break;
        case 3:
            selected = 6;
            break;
        case 4:
            if (hasAdvantage)
                advantage = 0;
            else
                advantage = 1;
            break;
        case 5:
            if (hasDisadvantage)
                advantage = 0;
            else
                advantage = -1;
            break;
        case 6:
            rollnow = true;
            break;
        }
        break;
    case sk_Add:
        if (selModSign)
            sign_positive = true;
        break;
    case sk_Sub:
        if (selModSign)
            sign_positive = false;
        break;
    case sk_Chs:
        if (selModSign)
            sign_positive = !sign_positive;
        break;
    }
}

void parseInput()
{
    sk_key_t key = getInput();
    parseInput(key);
}

int rollDice()
{
    int n = modifier * (sign_positive ? 1 : -1);
    for (unsigned int i = 0; i < number; i++)
    {
        if (hasDisadvantage)
        {
            uint32_t x = randInt(1, sides), y = randInt(1, sides);
            n += (x < y ? x : y);
        }
        else if (hasAdvantage)
        {
            uint32_t x = randInt(1, sides), y = randInt(1, sides);
            n += (x > y ? x : y);
        }
        else
        {
            n += randInt(1, sides);
        }
    }
    return n;
}

void showRolledDice(int rolled)
{
    gfx_FillScreen(WHITE);
    gfx_PrintCenteredScaledString("The dice you rolled is:", 80, 1.75, RED, WHITE);
    sprintf(str_buffer, "%d", rolled);
    gfx_PrintCenteredScaledString(str_buffer, 120, 1.5, BLACK, WHITE);
    gfx_PrintCenteredString("[Press Enter/2nd to roll again]", 180);
    gfx_PrintCenteredString("[Press Mode to quit]", 190);
    gfx_PrintCenteredString("[Press any key to modify]", 200);
}

int main()
{
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);
    gfx_SetTransparentColor(WHITE);
    gfx_SetTextFGColor(BLACK);
    gfx_SetMonospaceFont(8);
    gfx_SetTextConfig(gfx_text_clip);

    memcpy(Advantage, AdvantageGray, AdvantageGray_size);
    gfx_ColorSprite(Advantage, BLACK, WHITE);
    memcpy(Disadvantage, DisadvantageGray, DisadvantageGray_size);
    gfx_ColorSprite(Disadvantage, BLACK, WHITE);

    drawScreen();
    gfx_BlitBuffer();

    while (!exited)
    {
        parseInput();

        while (rollnow)
        {
            rollnow = false;
            selected = 0;
            int n = rollDice();
            showRolledDice(n);
            gfx_BlitBuffer();
            sk_key_t key;
            getKeyPress(key);
            switch (key)
            {
            case sk_2nd:
            case sk_Enter:
                rollnow = true;
                break;
            case sk_Mode:
                exited = true;
                break;
            }
        }

        drawScreen();
        gfx_BlitBuffer();
    }

    gfx_End();

    return 0;
}
