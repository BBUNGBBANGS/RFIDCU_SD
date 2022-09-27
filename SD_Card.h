#ifndef _SD_CARD_H
#define _SD_CARD_H

#include "Common.h"
#include "diskio.h"
#include "fatfs.h"
#include <stdint.h>
#include <stdio.h>

#define SDCARD_CS_GROUP             (GPIOA)
#define SDCARD_CS_PIN               (SPI_nCS)

#define SDCARD_DETECT_GROUP         (GPIOB)
#define SDCARD_DETECT_PIN           (nINP_SD_INSERT)

#define SDCARD_SELECT_GROUP         (GPIOB)
#define SDCARD_SELECT_PIN           (nSEL_SD_MEM)

#define SD_CARD_CHIP_SELECT_ENABLE      (1)
#define SD_CARD_CHIP_SELECT_DISABLE     (0)

#define SD_CARD_INSERTED                (0)

#define SPI_TIMEOUT 1000

/* Definitions for MMC/SDC command */
#define CMD0     (0x40+0)     /* GO_IDLE_STATE */
#define CMD1     (0x40+1)     /* SEND_OP_COND */
#define CMD8     (0x40+8)     /* SEND_IF_COND */
#define CMD9     (0x40+9)     /* SEND_CSD */
#define CMD10    (0x40+10)    /* SEND_CID */
#define CMD12    (0x40+12)    /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    /* SET_BLOCKLEN */
#define CMD17    (0x40+17)    /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    /* WRITE_BLOCK */
#define CMD25    (0x40+25)    /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    /* APP_CMD */
#define CMD58    (0x40+58)    /* READ_OCR */

extern void SD_Card_Init(void);
extern void SD_Card_Data_Write(void);
extern void SDTimer_Handler(void);

DSTATUS SD_disk_initialize (BYTE pdrv);
DSTATUS SD_disk_status (BYTE pdrv);
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

#endif