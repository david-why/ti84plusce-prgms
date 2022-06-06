#include "condrive.h"
#include <tice.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static usb_error_t handle_usb_event(usb_event_t event, void *event_data,
                                    usb_callback_data_t *callback_data)
{
    usb_error_t err;
    if ((err = srl_UsbEventCallback(event, event_data, callback_data)) != USB_SUCCESS)
        return err;
    if (event == USB_DEVICE_CONNECTED_EVENT)
    {
        usb_device_t device = event_data;
        usb_ResetDevice(device);
    }
    if (event == USB_HOST_CONFIGURE_EVENT || event == USB_DEVICE_ENABLED_EVENT /*|| (event == USB_DEVICE_ENABLED_EVENT && !(usb_GetRole() & USB_ROLE_DEVICE))*/)
    {
        if (callback_data->has_srl_device)
            return USB_SUCCESS;
        usb_device_t device = NULL;
        while ((device = usb_FindDevice(NULL, device, USB_SKIP_HUBS)))
        {
            srl_error_t error = srl_Open(&callback_data->srl, device, callback_data->srl_buf, sizeof(callback_data->srl_buf), SRL_INTERFACE_ANY, 115200);
            if (error)
            {
                if (callback_data->srl.type != 0)
                    goto inited_srl;
                continue;
            }
        inited_srl:
            callback_data->has_srl_device = true;
            break;
        }
        return USB_SUCCESS;
    }
    if (event == USB_DEVICE_DISCONNECTED_EVENT)
    {
        usb_device_t device = event_data;
        if (device == callback_data->srl.dev && callback_data->has_srl_device)
        {
            srl_Close(&callback_data->srl);
            memset(&callback_data->srl, 0, sizeof(callback_data->srl));
            callback_data->has_srl_device = false;
        }
    }
    return USB_SUCCESS;
}

con_error_t con_Init(con_device_t *con, uint8_t timeout)
{
    const usb_standard_descriptors_t *desc = srl_GetCDCStandardDescriptors();
    usb_error_t usb_error = usb_Init(handle_usb_event, con, desc, USB_DEFAULT_INIT_FLAGS);
    os_GetCSC();
    if (usb_error)
    {
        usb_Cleanup();
        return usb_error;
    }
    uint32_t begin = rtc_Time();
    while (rtc_Time() - begin < timeout)
    {
        usb_HandleEvents();
        if (con->has_srl_device)
            return CON_SUCCESS;
        if (os_GetCSC())
            goto timeout;
    }
timeout:
    usb_Cleanup();
    return CON_ERROR_TIMEOUT;
}

con_error_t con_HandleEvents(con_device_t *con __attribute__((unused)))
{
    usb_HandleEvents();
    if (!con->has_srl_device)
        return CON_ERROR_NO_SERIAL;

chk_event:;
    uint8_t event;
    int ret = srl_Read(&con->srl, &event, 1);
    if (ret < 0)
        return ret;
    else if (ret)
    {
        if (con->callbacks[event])
            con->callbacks[event](con);
        goto chk_event;
    }
    return CON_SUCCESS;
}

void con_Deinit(con_device_t *con __attribute__((unused)))
{
    usb_Cleanup();
}

int con_Read(con_device_t *con, void *data, int length)
{
    usb_HandleEvents();
    if (!con->has_srl_device)
        return CON_ERROR_NO_SERIAL;
    return srl_Read(&con->srl, data, length);
}

int con_EmitEvent(con_device_t *con, char event, void *data, int length)
{
    usb_HandleEvents();
    if (!con->has_srl_device)
        return CON_ERROR_NO_SERIAL;
    int ret = srl_Write(&con->srl, &event, 1);
    if (ret <= 0)
        return ret;
    return srl_Write(&con->srl, data, length) + 1;
}

con_error_t con_RegisterCallback(con_device_t *con, char event, con_callback_t callback)
{
    con->callbacks[(uint8_t)event] = callback;
    return CON_SUCCESS;
}
