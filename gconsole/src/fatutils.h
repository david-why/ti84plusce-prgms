#ifndef _FATUTILS_H
#define _FATUTILS_H

#ifndef MAX_PARTITIONS
#define MAX_PARTITIONS 10
#endif /* MAX_PARTITIONS */

typedef struct global global_t;
typedef unsigned int uint24_t;
#define usb_callback_data_t global_t
#define fat_callback_data_t msd_t

#include <tice.h>
#include <usbdrvce.h>
#include <msddrvce.h>
#include <fatdrvce.h>
#include <stdlib.h>
#include <string.h>
#include "TINYSTL/vector.h"

struct global
{
    usb_device_t usb;
    msd_t msd;
};
enum
{
    USB_RETRY_INIT = USB_USER_ERROR
};

/**
 * Initializes the USB, MSD, and FAT drivers. 
 * This function always opens the first available partition in the drive.
 * 
 * @param timeout The timeout, in seconds, to wait before returning timeout
 * error (1).
 * 
 * @returns Error status: 0=success, 1=timeout, 2=USB error, 3=MSD error,
 * 4=FAT error
 */
uint8_t fatutil_Init(unsigned int timeout);

/**
 * Deinitializes the USB, MSD, and FAT drivers.
 * 
 * @returns Error status: 0=success
 */
uint8_t fatutil_Deinit();

typedef struct
{
    const char *dir;
    fat_dir_entry_t entry;
} fatutil_entry_t;

/**
 * Lists the files in a directory.
 * 
 * @param path The directory to list.
 * @param option The listing option. Can be FAT_LIST_FILEONLY,
 * FAT_LIST_DIRONLY, or FAT_LIST_ALL. Defaults to FAT_LIST_ALL.
 * 
 * @returns The file entries found.
 */
tinystl::vector<fatutil_entry_t> fatutil_ListDir(const char *path, fat_list_option_t option = FAT_LIST_ALL);

typedef struct
{
    uint32_t pos;
    fat_file_t file;
    uint8_t buf[FAT_BLOCK_SIZE];
} fatutil_file_t;

/**
 * Opens a file in the FAT drive.
 * 
 * @param path The path of the file, beginning with "/".
 * 
 * @returns File pointer, or NULL on error. Can be used in various file I/O
 * functions.
 */
fatutil_file_t *fatutil_Open(const char *path);

/**
 * Opens a file in the FAT drive.
 * 
 * @param entry The file entry returned by fatutil_ListDir.
 * 
 * @returns File pointer, or NULL on error. Can be used in various file I/O
 * functions.
 */
fatutil_file_t *fatutil_Open(const fatutil_entry_t &entry);

/**
 * Get the file size, in bytes.
 * 
 * @param entry The entry to get the size of.
 * 
 * @returns The size of the file, in bytes.
 */
uint32_t fatutil_GetSize(const fatutil_entry_t &entry);

/**
 * Get the file size, in bytes.
 * 
 * @param file The file to get the size of.
 * 
 * @returns The size of the file, in bytes.
 */
uint32_t fatutil_GetSize(fatutil_file_t *file);

/**
 * Sets a file's size, in bytes.
 * 
 * @param file The file to set the size of.
 * @param size The size of the file, in bytes.
 * 
 * @returns FAT error (0=success).
 */
fat_error_t fatutil_SetSize(fatutil_file_t *file, uint32_t size);

/**
 * Returns the position in the file.
 * 
 * @param file The file to tell.
 * 
 * @returns The position in the file.
 */
uint32_t fatutil_Tell(const fatutil_file_t *file);

/**
 * Seeks to a potision in a file.
 * 
 * @param file The file to seek.
 * @param pos The position to seek to.
 * 
 * @returns Error status: 0=success, 1=pos>size
 */
uint8_t fatutil_Seek(fatutil_file_t *file, uint32_t pos);

/**
 * Reads file bytes from a file.
 * 
 * @param file The file to read from.
 * @param size The number of bytes to read.
 * @param ptr The pointer to store read bytes.
 * 
 * @return Bytes read, should be equal to or less than \p size.
 */
uint32_t fatutil_Read(fatutil_file_t *file, uint32_t size, void *ptr);

/**
 * Closes an open file.
 * 
 * @param file The file to close.
 * 
 * @returns Error status: 0=success, 1=close error
 */
uint8_t fatutil_Close(fatutil_file_t *file);

#endif /* _FATUTILS_H */
