/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2024 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32
#include "Klangstrom_BSP_KLST_PANDA_STM32-Config.h"
#ifdef KLST_PANDA_ENABLE_SD_CARD

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "main.h"
#include "fatfs.h"
#include "sdmmc.h"

#include "KlangstromSDCard.h"

extern SD_HandleTypeDef hsd2;

// #ifdef __cplusplus
// extern "C" {
// #endif

static void println(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n\r");
    va_end(args);
}

//static void read_file_example() {
//    FRESULT res;          // Result of the file operations
//    FIL     file;         // File object
//    char    buffer[4096]; // Buffer to store the read data
//    UINT    bytesRead;    // Number of bytes read
//
//    // Mount the filesystem
//    FATFS fs;
//    res = f_mount(&fs, "", 0);
//    if (res != FR_OK) {
//        // Handle the error
//        println("Failed to mount filesystem");
//        return;
//    }
//
//    // Open the file for reading
//    res = f_open(&file, "KLST.TXT", FA_READ);
//    if (res != FR_OK) {
//        // Handle the error
//        println("Failed to open file: %i", res);
//        f_mount(NULL, "", 0); // Unmount the filesystem
//        return;
//    }
//
//    // Read data from the file
//    res = f_read(&file, buffer, sizeof(buffer) - 1, &bytesRead);
//    if (res != FR_OK) {
//        // Handle the error
//        println("Failed to read file");
//    } else {
//        // // Null-terminate the buffer to make it a valid C string
//        // buffer[bytesRead] = '\0';
//        // // Handle the data
//        // println("data read: %i", bytesRead);
//        // println("Data read from file: ");
//        // for (int i = 0; i < bytesRead; i++) {
//        //     printf("%c", buffer[i]);
//        // }
//        // println("");
//        println(buffer);
//    }
//
//    // Close the file
//    f_close(&file);
//
//    // Unmount the filesystem
//    f_mount(NULL, "", 0);
//}

// bool sd_card_open(const char* filepath) {
// if (f_open(&SDFile, "KLST.TXT", FA_READ) != FR_OK) {
//     println("SDCard: f_open failed");
//     return false;
// }
// return true;
// }

// static uint8_t          rtext[_MAX_SS];

// sdcard_check_status();
// sdcard_write_test_file(false);
//    println("SDCard: NOTE that DMA is disabled.");
//    println("SDCard: if DMA is enabled DCache must handled properly");
//    println("SDCard: e.g stored in `section(\".dma_buffer\")`");

// static uint8_t  rtext[_MAX_SS];
// static FRESULT  res;                                                                                                                           /* FatFs function common result code */
// static uint32_t byteswritten;                                                                                                                  /* File write/read counts */
// static uint8_t  wtext[] = "Klangstrom (KLST) is an infrastructure to facilitate generative, networked, embedded sound + music + composition."; /* File write buffer */

// void sdcard_write_test_file(bool format_volume) {
//     // if (detected()) {
//     //     println("SDCard: card detected");
//     // } else {
//     //     println("SDCard: no card detected");
//     //     return;
//     // }

//     println("SDCard: mounting FS ...");
//     if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
//         println("SDCard: f_mount failed");
//         return;
//     }
//     if (format_volume) {
//         println("SDCard: creating volume ( this may take a while approx 6sec/1GB ) ...");
//         if (f_mkfs((TCHAR const*) SDPath, FM_ANY, 0, rtext, sizeof(rtext)) != FR_OK) {
//             println("SDCard: f_mkfs failed");
//             return;
//         }
//     }

//     println("SDCard: list all directories + files");
//     DIR     dir;
//     FILINFO fno;
//     FRESULT res = f_opendir(&dir, "/");
// if (res != FR_OK) {
//     println("SDCard: f_opendir failed : %i", res);
//     return;
// }

//     while (1) {
//         res = f_readdir(&dir, &fno);
//         if (res != FR_OK || fno.fname[0] == 0) {
//             break; // Break on error or end of dir
//         }

//         // Print the name of the file or directory
//         if (fno.fattrib & AM_DIR) {
//             println("        Directory: %s", fno.fname);
//         } else {
//             println("        File     : %s", fno.fname);
//         }
//     }
//     f_closedir(&dir);

// println("SDCard: opening file");
// if (f_open(&SDFile, "KLST.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
//     println("SDCard: f_open failed");
//     return;
// }
// println("SDCard: writing to file");
// res = f_write(&SDFile, wtext, strlen((char*) wtext), (UINT*) &byteswritten);
// if ((byteswritten == 0) || (res != FR_OK)) {
//     println("SDCard: f_write failed");
//     return;
// } else {
//     f_close(&SDFile);
// }
//     println("SDCard: ... unmounting FS");
//     f_mount(&SDFatFS, (TCHAR const*) NULL, 0);
// }

// bool sdcard_init() {
//     hsd2.Init.ClockDiv = SDMMC_INIT_CLK_DIV;
//     MX_SDMMC2_SD_Init();
//     MX_FATFS_Init();

//     if (HAL_SD_Init(&hsd2) != HAL_OK) {
//         println("SDCard: init failed");
//         return false;
//     }
//     println("SDCard: setup completed");

//     println("SDCard: read_file_example");
//     read_file_example();
//     return true;
// }

// #ifdef __cplusplus
// }
// #endif

// typedef enum {
//     FR_OK = 0,              /* (0) Succeeded */
//     FR_DISK_ERR,            /* (1) A hard error occurred in the low level disk I/O layer */
//     FR_INT_ERR,             /* (2) Assertion failed */
//     FR_NOT_READY,           /* (3) The physical drive cannot work */
//     FR_NO_FILE,             /* (4) Could not find the file */
//     FR_NO_PATH,             /* (5) Could not find the path */
//     FR_INVALID_NAME,        /* (6) The path name format is invalid */
//     FR_DENIED,              /* (7) Access denied due to prohibited access or directory full */
//     FR_EXIST,               /* (8) Access denied due to prohibited access */
//     FR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
//     FR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
//     FR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
//     FR_NOT_ENABLED,         /* (12) The volume has no work area */
//     FR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
//     FR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any problem */
//     FR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
//     FR_LOCKED,              /* (16) The operation is rejected according to the file sharing policy */
//     FR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
//     FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_LOCK */
//     FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
// } FRESULT;

bool SDCard::init() {
    hsd2.Init.ClockDiv = SDMMC_INIT_CLK_DIV;
    MX_SDMMC2_SD_Init();
    MX_FATFS_Init();

    if (HAL_SD_Init(&hsd2) != HAL_OK) {
        println("SDCard: init failed");
        return false;
    }
    println("SDCard: int completed");
    return true;
}

bool SDCard::reinit() {
    HAL_StatusTypeDef status = HAL_SD_InitCard(&hsd2);
    if (status != HAL_OK) {
        println("SDCard: card re-init failed");
        return false;
    }
    return true;
}

bool SDCard::detected() {
    if (!HAL_GPIO_ReadPin(_CARD_SDMMC_CHIP_DETECT_GPIO_Port, _CARD_SDMMC_CHIP_DETECT_Pin)) {
        return true;
    } else {
        return false;
    }
}

bool SDCard::status() {
    println("SDCard: checking status");

    if (detected()) {
        println("SDCard: card detected");
    } else {
        println("SDCard: no card detected");
        return false;
    }

    HAL_SD_CardStatusTypeDef card_status;
    HAL_StatusTypeDef        status = HAL_SD_GetCardStatus(&hsd2, &card_status);
    if (status != HAL_OK) {
        println("SDCard: get card status failed");
        return false;
    }
    println("SDCard: Card Status");
    println("SDCard: DataBusWidth ............ : %i", card_status.DataBusWidth);
    println("SDCard: SecuredMode ............. : %i", card_status.SecuredMode);
    println("SDCard: CardType ................ : %i", card_status.CardType);
    println("SDCard: ProtectedAreaSize ....... : %li", card_status.ProtectedAreaSize);
    println("SDCard: SpeedClass .............. : %i", card_status.SpeedClass);
    println("SDCard: PerformanceMove ......... : %i", card_status.PerformanceMove);
    println("SDCard: AllocationUnitSize ...... : %i", card_status.AllocationUnitSize);
    println("SDCard: EraseSize ............... : %i", card_status.EraseSize);
    println("SDCard: EraseTimeout ............ : %i", card_status.EraseTimeout);
    println("SDCard: EraseOffset ............. : %i", card_status.EraseOffset);
    println("SDCard: UhsSpeedGrade ........... : %i", card_status.UhsSpeedGrade);
    println("SDCard: UhsAllocationUnitSize ... : %i", card_status.UhsAllocationUnitSize);
    println("SDCard: VideoSpeedClass ......... : %i", card_status.VideoSpeedClass);

    HAL_SD_CardInfoTypeDef card_info;
    status = HAL_SD_GetCardInfo(&hsd2, &card_info);
    if (status != HAL_OK) {
        println("SDCard: get card info failed");
        return false;
    }
    println("SDCard: Card Info");
    println("SDCard: CardType ................ : %li", card_info.CardType);
    println("SDCard: CardVersion ............. : %li", card_info.CardVersion);
    println("SDCard: Class ................... : %li", card_info.Class);
    println("SDCard: RelCardAdd .............. : %li", card_info.RelCardAdd);
    println("SDCard: BlockNbr ................ : %li", card_info.BlockNbr);
    println("SDCard: BlockSize ............... : %li", card_info.BlockSize);
    println("SDCard: LogBlockNbr ............. : %li", card_info.LogBlockNbr);
    println("SDCard: LogBlockSize ............ : %li", card_info.LogBlockSize);
    println("SDCard: CardSpeed ............... : %li", card_info.CardSpeed);
    return true;
}

bool SDCard::mount() {
    println("SDCard: mounting FS ...");
    if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
        println("SDCard: f_mount failed");
        return false;
    }
    return true;
}

bool SDCard::unmount() {
    println("SDCard: ... unmounting FS");
    f_mount(&SDFatFS, (TCHAR const*) NULL, 0);
    return true;
}

bool SDCard::format() {
    BYTE work[_MAX_SS];
    println("SDCard: creating volume ( this may take a while approx 6sec/1GB ) ...");
    if (f_mkfs((TCHAR const*) SDPath, FM_ANY, 0, work, sizeof(work)) != FR_OK) {
        println("SDCard: f_mkfs failed");
        return false;
    }
    return true;
}

bool SDCard::list(std::string path, std::vector<std::string>& files, std::vector<std::string>& directories) {
    println("SDCard: list all directories + files");
    DIR     dir;
    FILINFO fno;
    FRESULT res = f_opendir(&dir, path.c_str());
    if (res != FR_OK) {
        println("SDCard: f_opendir failed : %i", res);
        return false;
    }

    while (1) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) {
            break; // Break on error or end of dir
        }

        // Print the name of the file or directory
        if (fno.fattrib & AM_DIR) {
            println("        Directory: %s", fno.fname);
            directories.push_back(fno.fname);
        } else {
            println("        File     : %s", fno.fname);
            files.push_back(fno.fname);
        }
    }
    f_closedir(&dir);
    return true;
}

bool SDCard::open_file(std::string filepath, uint8_t flags) {
    FRESULT res = f_open(&SDFile, filepath.c_str(), flags);
    if (res != FR_OK) {
        println("SDCard: f_open failed : %i", res);
        return false;
    }
    println("SDCard: opened file");
    return true;
}

uint32_t SDCard::write(uint8_t* bytes, uint32_t bytes_to_write) {
    println("SDCard: writing to file");
    uint32_t byteswritten;
    FRESULT  res = f_write(&SDFile, bytes, bytes_to_write, (UINT*) &byteswritten);
    if (res != FR_OK || (byteswritten == 0)) {
        println("SDCard: f_write failed : %i", res);
        return 0;
    }
    println("SDCard: written %i bytes", byteswritten);
    return byteswritten;
}

uint32_t SDCard::read(uint8_t* bytes, uint32_t bytes_to_read) {
    uint32_t bytesread;
    FRESULT  res = f_read(&SDFile, bytes, bytes_to_read, (UINT*) &bytesread);
    if (res != FR_OK) {
        println("SDCard: f_read failed: %i", res);
        return 0;
    }
    println("SDCard: read %i bytes", bytesread);
    return bytesread;
}

bool SDCard::close_file() {
    FRESULT res = f_close(&SDFile);
    if (res != FR_OK) {
        println("SDCard: closing file failed : %i", res);
        return false;
    }
    println("SDCard: closed file");
    return true;
}

#endif // KLST_PANDA_ENABLE_SD_CARD
#endif // KLST_PANDA_STM32
