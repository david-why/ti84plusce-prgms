#include <srldrvce.h>
#include <usbdrvce.h>
#include <tice.h>
#include <fileioc.h>
#include <keypadc.h>

#include <stdio.h>

usb_device_t device;
srl_device_t srl;
bool has_srl_device;
uint8_t srl_buf[512];
char strbuf[128];

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data __attribute__((unused)))
{
    sprintf(strbuf, "<%u>", event);
    os_PutStrFull(strbuf);
    /* Enable newly connected devices */
    if (event == USB_DEVICE_CONNECTED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE))
    {
        usb_device_t device = (usb_device_t)event_data;
        os_PutStrFull("device connected");
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
    kb_EnableOnLatch();

    ti_CloseAll();
    ti_var_t slot = ti_OpenVar(ti_Str1, "w", TI_STRING_TYPE);
    if (!slot)
        return 1;

    const usb_standard_descriptors_t *desc = srl_GetCDCStandardDescriptors();
    if (usb_Init(handle_usb_event, NULL, desc, USB_DEFAULT_INIT_FLAGS))
    {
        usb_Cleanup();
        os_PutStrFull("USB init error");
        delay(2000);
        return 1;
    }

    bool connected = false;
    size_t total_read = 0;
    os_PutStrFull("Waiting for USB connection");
    do
    {
        usb_HandleEvents();

        if (device && !has_srl_device)
        {
            if (srl_Open(&srl, device, srl_buf, sizeof(srl_buf), SRL_INTERFACE_ANY, 115200))
            {
                usb_Cleanup();
                os_PutStrFull("Serial open error");
                delay(2000);
                return 1;
            }

            has_srl_device = true;
            connected = true;

            os_PutStrFull("USB connected, receiving, press On to end");
            os_ClrHome();
        }

        if (has_srl_device)
        {
            uint8_t bytes_read;
            bytes_read = srl_Read(&srl, strbuf, 128);
            total_read += bytes_read;
            // srl_Write(&srl, strbuf, bytes_read);
            ti_Write(strbuf, 1, bytes_read, slot);
            sprintf(strbuf, "%u", total_read);
            os_HomeUp();
            os_PutStrFull(strbuf);
        }

        kb_ScanGroup(6);
    } while ((!connected || has_srl_device) && !kb_On);

    do
        kb_ScanGroup(6);
    while (kb_IsDown(kb_KeyEnter));
    ti_CloseAll();
    usb_Cleanup();
    return 0;
}
