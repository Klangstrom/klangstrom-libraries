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

#include <string.h>
#include "main.h"

#include "KLST_PANDA-SDCard.h"
#include "KLST_PANDA-SerialDebug.h"
#include "fatfs.h"

extern SD_HandleTypeDef hsd2;

FRESULT res; /* FatFs function common result code */
uint32_t byteswritten, bytesread; /* File write/read counts */
uint8_t wtext[] = "Klangstrom (KLST) is an infrastructure to facilitate generative, networked, embedded sound + music + composition."; /* File write buffer */
uint8_t rtext[_MAX_SS];/* File read buffer */

void sdcard_setup() {
    hsd2.Init.ClockDiv = SDMMC_INIT_CLK_DIV;
    if (HAL_SD_Init(&hsd2) != HAL_OK) {
        println("SDCard: init failed");
    }
    println("SDCard: setup completed");
//    println("SDCard: NOTE that DMA is disabled.");
//    println("SDCard: if DMA is enabled DCache must handled properly");
//    println("SDCard: e.g stored in `section(\".dma_buffer\")`");
}

bool sdcard_detected() {
    if (!HAL_GPIO_ReadPin(_CARD_SDMMC_CHIP_DETECT_GPIO_Port, _CARD_SDMMC_CHIP_DETECT_Pin)) {
        return true;
    } else {
        return false;
    }
}

void sdcard_check_status() {
    if (sdcard_detected()) {
        println("SDCard: card detected");
    } else {
        println("SDCard: no card detected");
        return;
    }

    HAL_SD_CardStatusTypeDef card_status;
    HAL_StatusTypeDef status = HAL_SD_GetCardStatus(&hsd2, &card_status);
    if (status != HAL_OK) {
        println("SDCard: get card status failed");
        return;
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
        return;
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

    status = HAL_SD_InitCard(&hsd2);
    if (status != HAL_OK) {
        println("SDCard: card init failed");
        return;
    }

    println("SDCard: checking status");
}

void sdcard_write_test_file(bool format_volume) {
    if (sdcard_detected()) {
        println("SDCard: card detected");
    } else {
        println("SDCard: no card detected");
        return;
    }

    println("SDCard: mounting FS ...");
    if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
        println("SDCard: f_mount failed");
        return;
    }
    if (format_volume) {
        println("SDCard: creating volume ( this may take a while approx 6sec/1GB ) ...");
        if (f_mkfs((TCHAR const*) SDPath, FM_ANY, 0, rtext, sizeof(rtext)) != FR_OK) {
            println("SDCard: f_mkfs failed");
            return;
        }
    }

    println("SDCard: list all directories + files");
    DIR dir;
    FILINFO fno;
    FRESULT res = f_opendir(&dir, "/");
    if (res != FR_OK) {
        println("SDCard: f_opendir failed : %i", res);
        return;
    }

    while (1) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break; // Break on error or end of dir

        // Print the name of the file or directory
        if (fno.fattrib & AM_DIR) {
            println("        Directory: %s", fno.fname);
        } else {
            println("        File     : %s", fno.fname);
        }
    }
    f_closedir(&dir);

    println("SDCard: opening file");
    if (f_open(&SDFile, "KLST.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
        println("SDCard: f_open failed");
        return;
    }
    println("SDCard: writing to file");
    res = f_write(&SDFile, wtext, strlen((char*) wtext), (void*) &byteswritten);
    if ((byteswritten == 0) || (res != FR_OK)) {
        println("SDCard: f_write failed");
        return;
    } else {
        f_close(&SDFile);
    }
    println("SDCard: ... unmounting FS");
    f_mount(&SDFatFS, (TCHAR const*) NULL, 0);
}

#endif // KLST_PANDA_ENABLE_SD_CARD

#endif KLST_PANDA_STM32
