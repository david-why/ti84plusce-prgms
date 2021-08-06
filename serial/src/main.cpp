#define HAS_PRINTF 1
#include <stdio.h>

#include <srldrvce.h>
#include <tice.h>

unsigned char srl_buf[512];
unsigned char in_buf[512];
srl_device_t srl;
bool has_srl_device;
bool run = true;
usb_device_t device;
/**
 * - 0: no escape characters received
 * - 1: received ESC only
 * - 2: received ESC and [
 * - 3: received ESC [ 3 (setting foreground)
 * - 4: received ESC [ 4 (setting background)
 * - 5: waiting for "m" to end
 */
int escape_status = 0;

bool print_data = true;
bool bold;
uint8_t fgcolor;
uint8_t bgcolor;

void moveCursor(unsigned int x, unsigned int y)
{
}

void handle_char(char c)
{
    // Handle special control codes first.
    if (c == 1) // quit
    {
        run = false;
    }
    else if (c == 2) // clear
    {
        os_ClrHome();
        os_SetCursorPos(0, 0);
    }
    else if (c == 8 || c == 0x7f) // backspace, delete
    {
        unsigned int x, y;
        os_GetCursorPos(&y, &x);
        if (x)
            x--;
        else if (y)
        {
            y--;
            x = 25;
        }
        os_SetCursorPos(y, x);
        printf(" ");
        os_SetCursorPos(y, x);
    }
    else if (c == '\x1b' && escape_status == 0)
    {
        escape_status = 1;
    }
    else if (c == '[' && escape_status == 1)
    {
        escape_status = 2;
    }
    else if (escape_status == 2)
    {
        switch (c)
        {
        case '0':
            escape_status = 5;
            bold = false;
            fgcolor = 0;
            bgcolor = 0;
            break;
        case '3':
            escape_status = 3;
            break;
        case '4':
            escape_status = 4;
            break;
        case 'A':
            moveCursor(0, -1);
            escape_status = 0;
            break;
        case 'B':
            moveCursor(0, 1);
            escape_status = 0;
            break;
        case 'C':
            moveCursor(1, 0);
            escape_status = 0;
            break;
        case 'D':
            moveCursor(-1, 0);
            escape_status = 0;
            break;
        }
    }
    else if (escape_status == 3)
    {
        if (c >= '0' && c <= '7')
        {
            fgcolor = c - '0';
            escape_status = 5;
        }
    }
    else if (escape_status == 4)
    {
        if (c >= '0' && c <= '7')
        {
            bgcolor = c - '0';
            escape_status = 5;
        }
    }

    else
    {
        escape_status = 0;
        if (print_data)
        {
            if (c == 13) // CR, change to LF
                c = '\n';
            if ((c >= ' ' && c <= '~') || c == '\n')
            {
                printf("%c", c);
            }
            else if (c == 127 || c == 8) // DEL, BS
            {
                unsigned int x, y;
                os_GetCursorPos(&y, &x);
                if (x)
                    x--;
                else if (y)
                {
                    y--;
                    x = 25;
                }
                os_SetCursorPos(y, x);
                printf(" ");
                os_SetCursorPos(y, x);
            }
            else
            {
                printf("<%02x>", (unsigned char)c);
            }
        }
    }
}

static usb_error_t handle_usb_event(usb_event_t event, void *event_data, usb_callback_data_t *callback_data __attribute__((unused)))
{
    /* Enable newly connected devices */
    if (event == USB_DEVICE_CONNECTED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE))
    {
        usb_device_t device = (usb_device_t)event_data;
        usb_ResetDevice(device);
    }
    if (event == USB_HOST_CONFIGURE_EVENT)
    {
        usb_device_t host = usb_FindDevice(NULL, NULL, USB_SKIP_HUBS);
        if (host)
            device = host;
    }
    /* When a device is connected, or when connected to a computer */
    if ((event == USB_DEVICE_ENABLED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE)))
    {
        device = (usb_device_t)event_data;
    }
    if (event == USB_DEVICE_DISCONNECTED_EVENT)
    {
        srl_Close(&srl);
        has_srl_device = false;
        device = NULL;
    }

    return USB_SUCCESS;
}

int main()
{
    os_ClrHome();
    if (usb_Init(handle_usb_event, NULL, srl_GetCDCStandardDescriptors(), USB_DEFAULT_INIT_FLAGS))
    {
        usb_Cleanup();
        printf("USB error, quitting\n");
        while (!os_GetCSC())
            delay(10);
        return 1;
    }

    while (run)
    {
        usb_HandleEvents();

        if (device && !has_srl_device)
        {
            if (srl_Open(&srl, device, srl_buf, sizeof(srl_buf), SRL_INTERFACE_ANY, 115200))
            {
                printf("Serial error, quitting\n");
                while (!os_GetCSC())
                    delay(10);
                return 1;
            }
            has_srl_device = true;
        }

        if (has_srl_device)
        {
            size_t bytes_read = srl_Read(&srl, in_buf, sizeof(in_buf));
            if (bytes_read)
            {
                for (size_t i = 0; i < bytes_read; i++)
                {
                    unsigned char c = in_buf[i];
                    handle_char(c);
                }
            }
        }

        sk_key_t key = os_GetCSC();
        switch (key)
        {
        case 0:
            break;
        case sk_Clear:
            run = false;
            break;
        default:
            if (key == sk_Div)
                key = 0x50;
            else if (key == sk_Add)
                key = 0x51;
            if (has_srl_device)
                srl_Write(&srl, &key, 1);
            break;
        }
    }

    usb_Cleanup();

    return 0;
}
