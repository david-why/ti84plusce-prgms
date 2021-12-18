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
#include "TINYSTL/unordered_map.h"

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

#define BLKSIZE 1024

global_t global;
char buf[BLKSIZE];
msd_partition_t partitions[MAX_PARTITIONS];
fat_t fat;
fat_dir_entry_t entry;
fat_file_t file;
unsigned int curblock = -1;

uint8_t *symbol_set;

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
    return head.symbol_height;
}
uint8_t get_width()
{
    return head.symbol_width;
}
uint8_t get_space()
{
    return head.line_space;
}
uint8_t get_cols()
{
    return head.page_cols;
}
uint8_t get_rows()
{
    return head.page_rows;
}
uint16_t get_symbols()
{
    return ((head.symbols % 256) << 8) + (head.symbols >> 8);
}
uint16_t get_pages()
{
    return ((head.pages % 256) << 8) + (head.pages >> 8);
}
uint8_t get_symbol_row(uint16_t sym, uint8_t row)
{
    return symbol_set[9 + sym * get_height() + row];
}
void readblk(unsigned int blk)
{
    if (curblock != blk)
    {
        fat_SetPos(&file, blk);
        fat_Read(&file, BLKSIZE / FAT_BLOCK_SIZE, buf);
        curblock = blk;
    }
}
uint8_t get_char(uint32_t page, uint16_t row, uint16_t col)
{
    uint32_t offset = (uint32_t)get_symbols() * get_height() + (page * get_rows() + row) * get_cols() + col + 9;
    sprintf(buf, "blk:%u", offset/BLKSIZE);
    os_PutStrFull(buf);
    os_NewLine();
    readblk(offset / BLKSIZE);
    return buf[offset % BLKSIZE];
}

bool init_book()
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
    err = fat_Open(&file, &fat, "/BOOK.TIB");
    if (err != FAT_SUCCESS)
    {
        sprintf(buf, "open file error %u", err);
        os_PutStrFull(buf);
        goto fat_error;
    }

    readblk(0);
    memcpy(&head, buf, sizeof(head));
    size_t sset_blks;
    sset_blks = ((get_symbols() * get_height() + sizeof(head)) + FAT_BLOCK_SIZE - 1) / FAT_BLOCK_SIZE;
    symbol_set = (uint8_t *)malloc(sset_blks * FAT_BLOCK_SIZE);
    if (symbol_set == NULL)
    {
        sprintf(buf, "malloc symbol_set fail %u", sset_blks * FAT_BLOCK_SIZE);
        os_PutStrFull(buf);
        goto fat_error;
    }
    fat_SetPos(&file, 0);
    fat_Read(&file, sset_blks, symbol_set);

    // void (*print)(const char *, unsigned int);
    // print = [](const char *s, unsigned int n)
    // {
    //     sprintf(buf, "%s: %u", s, n);
    //     os_PutStrFull(buf);
    //     os_NewLine();
    // };
    // print("size", fat_GetSize(&file));
    // print("height", get_height());
    // print("width", get_width());
    // print("space", get_space());
    // print("rows", get_rows());
    // print("cols", get_cols());
    // print("symbols", get_symbols());
    // print("pages", get_pages());
    // print("pos", fat_GetPos(&file));
    // sprintf(buf, "%u %u %u %u", get_char(0, 0, 0), get_char(0, 0, 1), get_char(0, 0, 2), get_char(0, 0, 3));
    // os_PutStrFull(buf);
    // readblk(1);
    // sprintf(buf, "%u %u %u %u", (uint8_t)buf[512+0], (uint8_t)buf[512+1], (uint8_t)buf[512+2], (uint8_t)buf[512+3]);
    // os_PutStrFull(buf);
    // os_NewLine();
    readblk(110);
    sprintf(buf, "%u %u %u %u %u", (uint8_t)buf[0], (uint8_t)buf[1], (uint8_t)buf[2], (uint8_t)buf[3], fat_GetPos(&file));
    os_PutStrFull(buf);
    os_NewLine();
    while (!os_GetCSC())
        ;
    goto fat_error;

    return true;

fat_error:
    // close the filesystem
    fat_Deinit(&fat);

msd_error:
    // close the msd device
    msd_Close(&global.msd);

usb_error:
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
