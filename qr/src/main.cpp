#define MAX_VERSION 10
#include "qrcode.h"
#include "textinput.h"
#include <string.h>
#include <stdio.h>
#include <tice.h>
#include <graphx.h>
#include <fileioc.h>

char buffer[272];
constexpr uint16_t qrsize = ((4 * MAX_VERSION + 17) * (4 * MAX_VERSION + 17) + 7) / 8;
uint8_t qrcodeBytes[qrsize];

struct save
{
    struct val
    {
        bool used;
        QRCode qrcode;
        char name[9];
        uint8_t data[qrsize];
        val() : used(false) {}
    };
    bool used;
    val codes[10];
    save() : used(false) {}
};

constexpr uint8_t getver(unsigned int read)
{
    if (read <= 17)
        return 1;
    if (read <= 32)
        return 2;
    if (read <= 53)
        return 3;
    if (read <= 78)
        return 4;
    if (read <= 106)
        return 5;
    if (read <= 134)
        return 6;
    if (read <= 154)
        return 7;
    if (read <= 192)
        return 8;
    if (read <= 230)
        return 9;
    return 10;
}

constexpr int8_t toint(sk_key_t key)
{
    if (key == sk_1)
        return 1;
    if (key == sk_2)
        return 2;
    if (key == sk_3)
        return 3;
    if (key == sk_4)
        return 4;
    if (key == sk_5)
        return 5;
    if (key == sk_6)
        return 6;
    if (key == sk_7)
        return 7;
    if (key == sk_8)
        return 8;
    if (key == sk_9)
        return 9;
    if (key == sk_0)
        return 0;
    return -1;
}

QRCode *qrcode;

void getqrinput()
{
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("Enter text, <272 char:", 10, 10);
    gfx_SetTextScale(1, 1);
    gfx_SwapDraw();
    int read = getinput(buffer, 272, 10, 50, sk_Enter, sk_Mode, "https://", "http://", "mailto:", "WIFI:T:");
    if (read < 0)
    {
        gfx_End();
        exit(0);
    }
    gfx_FillScreen(255);
    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(224);
    gfx_PrintStringXY("Generating, please", 20, 50);
    gfx_PrintStringXY("wait...", 20, 70);
    gfx_PrintStringXY("QRCode Version ", 20, 90);
    const uint8_t version = getver(read);
    gfx_PrintUInt(version, 1);
    gfx_SetTextScale(1, 1);
    gfx_SwapDraw();
    qrcode = new QRCode();
    qrcode_initText(qrcode, qrcodeBytes, version, ECC_LOW, buffer);
}

int main()
{
    ti_CloseAll();
    ti_var_t var = ti_Open("QRCODESV", "a");
    if (ti_Tell(var) == 0)
    {
        save *s = new save();
        ti_Write(s, sizeof(save), 1, var);
        ti_SetArchiveStatus(true, var);
        free(s);
    }
    ti_Close(var);
    var = ti_Open("QRCODESV", "r");
    save *s = new save();
    ti_Read(s, sizeof(save), 1, var);
    ti_Close(var);

    gfx_Begin();
    gfx_SetDrawBuffer();
    if (s->used)
    {
        gfx_PrintStringXY("Select:", 10, 10);
        gfx_PrintStringXY("[enter] New QRCode", 10, 10 + 12 * 1);
        for (int i = 0; i < 10; i++)
        {
            if (s->codes[i].used)
            {
                sprintf(buffer, "[%d] %s", i, s->codes[i].name);
                gfx_PrintStringXY(buffer, 10, 10 + 12 * 2 + i * 12);
            }
        }
        gfx_SwapDraw();
        while (true)
        {
            sk_key_t key;
            while (!(key = os_GetCSC()))
                delay(10);
            if (key == sk_Enter)
            {
                getqrinput();
                break;
            }
            if (key == sk_Clear)
            {
                gfx_End();
                return 0;
            }
            int8_t val = toint(key);
            if (val == -1 || !s->codes[val].used)
                continue;
            qrcode = new QRCode();
            memcpy(qrcode, &s->codes[val].qrcode, sizeof(QRCode));
            qrcode->modules = qrcodeBytes;
            memcpy(qrcodeBytes, s->codes[val].data, qrsize);
            break;
        }
    }
    else
        getqrinput();

    gfx_FillScreen(255);
    const uint8_t scale = LCD_HEIGHT / qrcode->size;
    const uint16_t top = (LCD_HEIGHT - qrcode->size * scale) / 2;
    const unsigned int left = (LCD_WIDTH - qrcode->size * scale) / 2;
    for (uint8_t y = 0; y < qrcode->size; y++)
        for (uint8_t x = 0; x < qrcode->size; x++)
            if (qrcode_getModule(qrcode, x, y))
                gfx_FillRectangle(left + x * scale, top + y * scale, scale, scale);
    gfx_SwapDraw();
    sk_key_t key;
    while (!(key = os_GetCSC()))
        delay(10);
    if (key == sk_Store)
    {
        gfx_FillScreen(255);
        gfx_SetTextFGColor(0);
        gfx_PrintStringXY("Select slot:", 10, 10);
        for (int i = 0; i < 10; i++)
        {
            if (s->codes[i].used)
                sprintf(buffer, "[%d] %s", i, s->codes[i].name);
            else
                sprintf(buffer, "[%d] <new>", i);
            gfx_PrintStringXY(buffer, 10, 10 + 12 * 1 + i * 12);
        }
        gfx_SwapDraw();
        while (true)
        {
            sk_key_t key;
            while (!(key = os_GetCSC()))
                delay(10);
            if (key == sk_Clear)
                break;
            int8_t val = toint(key);
            if (val == -1)
                continue;
            memcpy(&s->codes[val].qrcode, qrcode, sizeof(QRCode));
            memcpy(s->codes[val].data, qrcodeBytes, qrsize);
            gfx_FillScreen(255);
            gfx_SetTextScale(2, 2);
            gfx_PrintStringXY("Enter name, <9 char:", 10, 10);
            gfx_SetTextScale(1, 1);
            gfx_SwapDraw();
            getinput(s->codes[val].name, 9, 10, 50, sk_Enter, 0);
            s->codes[val].used = true;
            s->used = true;
            ti_CloseAll();
            var = ti_Open("QRCODESV", "w");
            ti_Write(s, sizeof(save), 1, var);
            ti_SetArchiveStatus(true, var);
            ti_Close(var);
            break;
        }
    }
    gfx_End();
    free(s);
    return 0;
}
