#ifndef HAS_PRINTF // Make VSCode happy.
#define HAS_PRINTF 1
#endif

#include <stdio.h>
#include <string.h>

#include <srldrvce.h>
#include <tice.h>

unsigned char srl_buf[512];
unsigned char in_buf[512];
srl_device_t srl;
usb_device_t device;
bool has_srl_device;

bool run = true;

const char *chars[] = {
    "", "|", "<-", "->", "|", "", "", "", "", "\r\n", "+", "-", "*", "/", "^", "\x03",
    "", "_", "3", "6", "9", ")", "tan", "vars", "", ".", "2", "5", "8", "(", "cos", "prgm",
    "stat", "0", "1", "4", "7", ",", "sin", "apps", "X,T,[,n", "", "sto->", "ln", "log", "^2", "^-1", "math",
    "", "graph", "trace", "zoom", "window", "y=", "2nd", "mode", "\x7f"};
bool alpha = false;
const char *alpha_chars[] = {
    "", "", "", "", "", "", "", "", "", "", "\"", "W", "R", "M", "H", "-\\( '_' )/-",
    "", "?", "=", "V", "Q", "L", "G", "", "", ":", "Z", "U", "P", "K", "F", "C",
    "", " ", "Y", "T", "O", "J", "E", "B", "", "", "X", "S", "N", "I", "D", "A",
    "", "F5", "F4", "F3", "F2", "F1", "", "", ""};
bool was_newline = false;

void handle_char(char c)
{
    if (c == '\r') // CR, ignore
    {
        if (was_newline)
            return;
        c = '\n';
    }
    if (c == '\n' && was_newline)
        return;
    if (c == '\x03')
    {
        os_ClrHome();
        os_SetCursorPos(0, 0);
        return;
    }
    if (c == '\x7f')
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
        return;
    }
    if ((c >= ' ' && c <= '~') || c == '\n')
        printf("%c", c);
    else
        printf("<%02x>", (unsigned char)c);
    if (c == '\n' || c == '\r')
        was_newline = true;
    else
        was_newline = false;
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

size_t _send(const char c)
{
    if (has_srl_device)
        return srl_Write(&srl, &c, 1);
    return 0;
}

size_t _send(const char *str)
{
    size_t ans = 0;
    for (size_t i = 0; i < strlen(str); i++)
        ans += _send(str[i]);
    return ans;
}

size_t _read()
{
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
        return bytes_read;
    }
    return 0;
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

        _read();

        sk_key_t key = os_GetCSC();
        const char *to_send;
        switch (key)
        {
        case 0:
            break;
        case sk_Alpha:
            alpha = !alpha;
            break;
        default:
            if (alpha)
            {
                to_send = alpha_chars[key];
                if (strlen(to_send) == 0)
                    to_send = chars[key];
            }
            else
            {
                to_send = chars[key];
            }
            _send(to_send);
            break;
        }
        if (boot_CheckOnPressed())
            run = false;
    }

    usb_Cleanup();

    return 0;
}
