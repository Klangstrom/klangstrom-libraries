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

    println("SDCard: mounting FS");
    if (f_mount(&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK) {
        println("SDCard: f_mount failed");
        return;
    }
    if (format_volume) {
        println("SDCard: creating volume ( this may take a while e.g 100s fo 16GB ) ...");
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
        println("SDCard: f_opendir failed");
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
