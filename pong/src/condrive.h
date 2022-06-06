#ifndef _CONDRIVE_H
#define _CONDRIVE_H

typedef struct con_device con_device_t;

#define usb_callback_data_t con_device_t
#include <usbdrvce.h>
#include <srldrvce.h>

typedef void (*con_callback_t)(con_device_t *);

struct con_device
{
    srl_device_t srl;
    uint8_t has_srl_device;
    char srl_buf[512];
    con_callback_t callbacks[256];
};

typedef enum
{
    CON_SUCCESS = 0,
    CON_ERROR_INVALID_PARAM = -1,
    CON_ERROR_USB_FAILED = -2,
    CON_ERROR_NOT_SUPPORTED = -3,
    CON_ERROR_INVALID_DEVICE = -4,
    CON_ERROR_INVALID_INTERFACE = -5,
    CON_ERROR_NO_MEMORY = -6,
    CON_ERROR_DEVICE_DISCONNECTED = -7,
    CON_ERROR_NO_SERIAL = -8,
    CON_ERROR_TIMEOUT = -9,
    CON_USB_IGNORE = 1,
    CON_USB_ERROR_SYSTEM,
    CON_USB_ERROR_INVALID_PARAM,
    CON_USB_ERROR_SCHEDULE_FULL,
    CON_USB_ERROR_NO_DEVICE,
    CON_USB_ERROR_NO_MEMORY,
    CON_USB_ERROR_NOT_SUPPORTED,
    CON_USB_ERROR_OVERFLOW,
    CON_USB_ERROR_TIMEOUT,
    CON_USB_ERROR_FAILED,
    CON_USB_USER_ERROR = 100,
} con_error_t;

con_error_t con_Init(con_device_t *con, uint8_t timeout_s);
con_error_t con_HandleEvents(con_device_t *con);
void con_Deinit(con_device_t *con);
int con_Read(con_device_t *con, void *data, int length);
int con_EmitEvent(con_device_t *con, char event, void *data, int length);
con_error_t con_RegisterCallback(con_device_t *con, char event, con_callback_t callback);

#endif