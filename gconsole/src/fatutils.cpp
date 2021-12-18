#include "fatutils.h"
#include <stdio.h>

static global_t global;
static msd_partition_t partitions[MAX_PARTITIONS];
static fat_t fat;
static bool inited;

static constexpr unsigned int pos2block(uint32_t pos) { return pos / FAT_BLOCK_SIZE; }
static constexpr unsigned int block2pos(uint32_t block) { return block * FAT_BLOCK_SIZE; }
static constexpr uint16_t pos2offset(uint32_t pos) { return pos % FAT_BLOCK_SIZE; }

static usb_error_t handleUsbEvent(usb_event_t event, void *event_data, usb_callback_data_t *global)
{
    switch (event)
    {
    case USB_DEVICE_DISCONNECTED_EVENT:
        if (global->usb)
            msd_Close(&global->msd);
        global->usb = NULL;
        inited = false;
        break;
    case USB_DEVICE_CONNECTED_EVENT:
        return usb_ResetDevice((usb_device_t)event_data);
    case USB_DEVICE_ENABLED_EVENT:
        global->usb = (usb_device_t)event_data;
        break;
    case USB_DEVICE_DISABLED_EVENT:
        return (usb_error_t)USB_RETRY_INIT;
    default:
        break;
    }
    return USB_SUCCESS;
}

uint8_t fatutil_Init(unsigned int timeout)
{
    uint8_t err, ret;
    memset(&global, 0, sizeof(global_t));
    uint32_t time = rtc_Time();
    do
    {
        global.usb = NULL;

        err = usb_Init(handleUsbEvent, &global, NULL, USB_DEFAULT_INIT_FLAGS);
        if (err != USB_SUCCESS)
        {
            ret = 2;
            goto usb_error;
        }

        while (err == USB_SUCCESS)
        {
            if (rtc_Time() - time > timeout)
            {
                ret = 1;
                goto usb_error;
            }
            if (global.usb != NULL)
                break;
            err = usb_HandleEvents();
        }
    } while (err == USB_RETRY_INIT);
    if (err != USB_SUCCESS)
    {
        ret = 2;
        goto usb_error;
    }
    err = msd_Open(&global.msd, global.usb);
    if (err != MSD_SUCCESS)
    {
        ret = 3;
        goto usb_error;
    }
    msd_info_t msdinfo;
    err = msd_Info(&global.msd, &msdinfo);
    if (err != MSD_SUCCESS)
    {
        ret = 3;
        goto msd_error;
    }
    uint8_t num_partitions;
    num_partitions = msd_FindPartitions(&global.msd, partitions, MAX_PARTITIONS);
    if (!num_partitions)
    {
        ret = 3;
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
            ret = 3;
            goto msd_error;
        }
    }

    inited = true;
    return 0;

msd_error:
    // close the msd device
    msd_Close(&global.msd);

usb_error:
    // cleanup usb
    usb_Cleanup();

    return ret;
}

uint8_t fatutil_Deinit()
{
    if (inited)
    {
        fat_Deinit(&fat);
        msd_Close(&global.msd);
        inited = false;
    }
    usb_Cleanup();
    return 0;
}

tinystl::vector<fatutil_entry_t> fatutil_ListDir(const char *path, fat_list_option_t option)
{
    tinystl::vector<fatutil_entry_t> files;
    if (!inited)
        return files;
    fat_dir_entry_t entry;
    unsigned int skip = 0;
    bool found;
    while ((found = fat_DirList(&fat, path, option, &entry, 1, skip++)))
        files.push_back(fatutil_entry_t{path, entry});
    return files;
}

fatutil_file_t *fatutil_Open(const char *path)
{
    if (!inited)
        return NULL;
    fatutil_file_t *file = (fatutil_file_t *)malloc(sizeof(fatutil_file_t));
    if (file == NULL)
        return NULL;
    uint8_t err = fat_Open(&file->file, &fat, path);
    if (err != FAT_SUCCESS)
    {
        free(file);
        return NULL;
    }
    file->pos = 0;
    fat_Read(&file->file, 1, file->buf);
    return file;
}

fatutil_file_t *fatutil_Open(const fatutil_entry_t &entry)
{
    if (!inited)
        return NULL;
    size_t dirlen = strlen(entry.dir), fnlen = strlen(entry.entry.filename);
    if (dirlen == 1)
        dirlen = 0;
    char *name = (char *)malloc(dirlen + 1 + fnlen + 1);
    if (name == NULL)
        return NULL;
    if (dirlen)
        strcpy(name, entry.dir);
    name[dirlen] = '/';
    strcpy(name + dirlen + 1, entry.entry.filename);
    fatutil_file_t *file = fatutil_Open(name);
    free(name);
    return file;
}

uint32_t fatutil_GetSize(const fatutil_entry_t &entry)
{
    return entry.entry.size;
}

uint32_t fatutil_GetSize(fatutil_file_t *file)
{
    return fat_GetSize((fat_file_t *)&file->file);
}

fat_error_t fatutil_SetSize(fatutil_file_t *file, uint32_t size)
{
    return fat_SetSize(&file->file, size);
}

uint32_t fatutil_Tell(const fatutil_file_t *file)
{
    return file->pos;
}

uint8_t fatutil_Seek(fatutil_file_t *file, uint32_t pos)
{
    if (pos > fatutil_GetSize(file))
        return 1;
    if (pos2block(file->pos) > pos2block(pos))
    {
        file->pos = 0;
        fat_SetPos(&file->file, 0);
        while (pos2block(file->pos) != pos2block(pos))
        {
            fat_Read(&file->file, 1, file->buf);
            file->pos += FAT_BLOCK_SIZE;
        }
        fat_Read(&file->file, 1, file->buf);
        file->pos = pos;
    }
    else if (pos2block(file->pos) == pos2block(pos))
    {
        file->pos = pos;
    }
    else
    {
        file->pos = block2pos(pos2block(file->pos));
        while (pos2block(file->pos) != pos2block(pos))
        {
            fat_Read(&file->file, 1, file->buf);
            file->pos += FAT_BLOCK_SIZE;
        }
        fat_Read(&file->file, 1, file->buf);
        file->pos = pos;
    }
    return 0;
}

uint32_t fatutil_Read(fatutil_file_t *file, uint32_t size, void *ptr)
{
    uint32_t fsize = fatutil_GetSize(file);
    if (fsize - file->pos < size)
        size = fsize - file->pos;
    uint16_t leftinblk = FAT_BLOCK_SIZE - pos2offset(file->pos);
    if (size < leftinblk)
        leftinblk = size;
    memcpy(ptr, file->buf + pos2offset(file->pos), leftinblk);
    file->pos += leftinblk;
    if (pos2offset(file->pos) == 0)
        fat_Read(&file->file, 1, file->buf);
    uint32_t read = leftinblk;
    while (size - read >= FAT_BLOCK_SIZE)
    {
        memcpy((uint8_t *)ptr + read, file->buf, FAT_BLOCK_SIZE);
        file->pos += FAT_BLOCK_SIZE;
        read += FAT_BLOCK_SIZE;
        fat_Read(&file->file, 1, file->buf);
    }
    memcpy((uint8_t *)ptr + read, file->buf, size - read);
    file->pos += size - read;
    return read;
}

uint8_t fatutil_Close(fatutil_file_t *file)
{
    uint8_t error = fat_Close(&file->file);
    free(file);
    return error == FAT_SUCCESS ? 0 : 1;
}
