/* book_fat.h: book in usb, name "book.tib" */

typedef struct global global_t;
#define usb_callback_data_t global_t
#define fat_callback_data_t msd_t

#include <tice.h>
#include <usbdrvce.h>
#include <msddrvce.h>
#include <fatdrvce.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTITIONS 10

struct global
{
    usb_device_t usb;
    msd_t msd;
};
enum
{
    USB_RETRY_INIT = USB_USER_ERROR
};
static usb_error_t handleUsbEvent(usb_event_t event, void *event_data, usb_callback_data_t *global)
{
    switch (event)
    {
    case USB_DEVICE_DISCONNECTED_EVENT:
        // print("usb device disconnected");
        if (global->usb)
            msd_Close(&global->msd);
        global->usb = NULL;
        break;
    case USB_DEVICE_CONNECTED_EVENT:
        // print("usb device connected");
        return usb_ResetDevice((usb_device_t)event_data);
    case USB_DEVICE_ENABLED_EVENT:
        global->usb = (usb_device_t)event_data;
        // print("usb device enabled");
        break;
    case USB_DEVICE_DISABLED_EVENT:
        // print("usb device disabled");
        return (usb_error_t)USB_RETRY_INIT;
    default:
        break;
    }
    return USB_SUCCESS;
}

uint32_t books;
global_t global;
char buf[512];
msd_partition_t partitions[MAX_PARTITIONS];
fat_t fat;
fat_dir_entry_t entry;
fat_file_t file;
unsigned int curblock = -1;
const char **names;

uint8_t *data;

#pragma pack(push, 1)
struct book_header
{
    uint8_t symbol_height;
    uint8_t symbol_width;
    uint8_t line_space;
    uint8_t page_rows;
    uint8_t page_cols;
    uint16_t symbols;
    uint16_t pages;
} head;
#pragma pack(pop)

uint8_t get_height()
{
    return data[0];
}
uint8_t get_width()
{
    return data[1];
}
uint8_t get_space()
{
    return data[2];
}
uint8_t get_cols()
{
    return data[4];
}
uint8_t get_rows()
{
    return data[3];
}
uint16_t get_symbols()
{
    return (((uint16_t)data[5]) << 8) + data[6];
}
uint16_t get_pages()
{
    return (((uint16_t)data[7]) << 8) + data[8];
}
uint8_t get_symbol_row(uint16_t sym, uint8_t row)
{
    return data[9 + sym * get_height() + row];
}
uint8_t get_char(uint32_t page, uint8_t row, uint8_t col)
{
    return data[9 + (uint32_t)get_symbols() * get_height() + page * get_rows() * get_cols() + row * get_cols() + col];
}

uint32_t n_books()
{
    memset(&global, 0, sizeof(global_t));
    uint8_t err;
    do
    {
        global.usb = NULL;

        err = usb_Init(handleUsbEvent, &global, NULL, USB_DEFAULT_INIT_FLAGS);
        if (err != USB_SUCCESS)
        {
            os_PutStrFull("usb_Init error");
            goto usb_error;
        }

        while (err == USB_SUCCESS)
        {
            if (global.usb != NULL)
                break;
            if (os_GetCSC())
                goto usb_error;
            err = usb_WaitForInterrupt();
        }
    } while (err == USB_RETRY_INIT);
    if (err != USB_SUCCESS)
    {
        os_PutStrFull("usb enable error.");
        goto usb_error;
    }
    err = msd_Open(&global.msd, global.usb);
    if (err != MSD_SUCCESS)
    {
        os_PutStrFull("failed opening msd");
        goto usb_error;
    }
    msd_info_t msdinfo;
    err = msd_Info(&global.msd, &msdinfo);
    if (err != MSD_SUCCESS)
    {
        os_PutStrFull("error getting msd info");
        goto msd_error;
    }
    uint8_t num_partitions;
    num_partitions = msd_FindPartitions(&global.msd, partitions, MAX_PARTITIONS);
    if (num_partitions < 1)
    {
        os_PutStrFull("no partitions found");
        goto msd_error;
    }
    fat.read = msd_Read;
    fat.write = msd_Write;
    fat.usr = &global.msd;
    for (uint8_t p = 0;;)
    {
        fat.first_lba = partitions[p].first_lba;
        fat.last_lba = partitions[p].last_lba;
        err = fat_Init(&fat);
        if (err == FAT_SUCCESS)
            break;
        p++;
        if (p >= num_partitions)
        {
            os_PutStrFull("no fat partitions");
            goto msd_error;
        }
    }

    unsigned int skip;
    skip = 0;
    while (fat_DirList(&fat, "/", FAT_LIST_FILEONLY, &entry, 1, skip++))
    {
        char *dot = strrchr(entry.filename, '.');
        if (dot == NULL)
            continue;
        if (!strcasecmp(dot + 1, "tib"))
        {
            books++;
            continue;
        }
        bool ok = true;
        for (char *pos = dot + 1; *pos; pos++)
        {
            if (*pos < '0' || *pos > '9')
            {
                ok = false;
                break;
            }
        }
        books += ok;
    }
    return books;

msd_error:
    // close the msd device
    msd_Close(&global.msd);

usb_error:
    // cleanup usb
    usb_Cleanup();

    return 0;
}
const char **get_names()
{
    data = (uint8_t *)malloc((8 + 1 + 3 + 1) * books);
    names = (const char **)malloc(sizeof(const char *) * books);
    if (data == NULL || names == NULL)
        os_ThrowError(OS_E_MEMORY);
    unsigned int skip = 0, found = 0;
    while (found != books && fat_DirList(&fat, "/", FAT_LIST_FILEONLY, &entry, 1, skip++))
    {
        char *dot = strrchr(entry.filename, '.');
        if (dot == NULL)
            continue;
        if (!strcasecmp(dot + 1, "tib"))
        {
            memcpy(data + (8 + 1 + 3 + 1) * found++, entry.filename, 8 + 1 + 3 + 1);
            continue;
        }
        bool ok = true;
        for (char *pos = dot + 1; *pos; pos++)
        {
            if (*pos < '0' || *pos > '9')
            {
                ok = false;
                break;
            }
        }
        if (ok)
            memcpy(data + (8 + 1 + 3 + 1) * found++, entry.filename, 8 + 1 + 3 + 1);
    }
    for (unsigned int i = 0; i < books; i++)
        names[i] = (char *)(data + (8 + 1 + 3 + 1) * i);
    return names;
}
bool init_book(const char *name)
{
    free(names);
    free(data);
    uint8_t err;
    strcpy(buf + 1, name);
    buf[0] = '/';
    err = fat_Open(&file, &fat, buf);
    if (err != FAT_SUCCESS)
    {
        sprintf(buf, "open file error %u", err);
        os_PutStrFull(buf);
        goto fat_error;
    }

    uint32_t blks, msize;
    blks = (fat_GetSize(&file) + FAT_BLOCK_SIZE - 1) / FAT_BLOCK_SIZE;
    msize = blks * FAT_BLOCK_SIZE;
    data = (uint8_t *)malloc(msize);
    if (data == NULL)
    {
        sprintf(buf, "malloc error %u", msize);
        os_PutStrFull(buf);
        goto fat_error;
    }
    fat_Read(&file, blks, data);

    return true;

fat_error:
    // close the filesystem
    fat_Deinit(&fat);
    // close the msd device
    msd_Close(&global.msd);
    // cleanup usb
    usb_Cleanup();

    return false;
}

void deinit_book()
{
    fat_Deinit(&fat);
    msd_Close(&global.msd);
    usb_Cleanup();
}
