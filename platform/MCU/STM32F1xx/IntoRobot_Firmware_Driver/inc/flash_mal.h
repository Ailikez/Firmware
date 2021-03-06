/**
 ******************************************************************************
 * @file    flash_mal.h
 * @author  Satish Nair
 * @version V1.0.0
 * @date    30-Jan-2015
 * @brief   Header for flash media access layer
 ******************************************************************************
 Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_MAL_H
#define __FLASH_MAL_H

#include "hw_config.h"
#include "flash_device_hal.h"

/* Includes ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/
#ifndef INTERNAL_FLASH_SIZE
#   error "INTERNAL_FLASH_SIZE not defined"
#endif

/* Internal Flash memory address where various firmwares are located */
#ifndef INTERNAL_FLASH_START
#define INTERNAL_FLASH_START        ((uint32_t)0x08000000)
#endif
/* Internal Flash End Address */
#define INTERNAL_FLASH_END      ((uint32_t)INTERNAL_FLASH_START+INTERNAL_FLASH_SIZE)	//For 512KB Internal Flash

//Bootloader firmware at the start of internal flash
#define USB_DFU_ADDRESS             INTERNAL_FLASH_START
//Main firmware begin address after 128KB (20K) from start of flash
#define CORE_FW_ADDRESS             ((uint32_t)0x08007000)
#define APP_START_MASK              ((uint32_t)0x2FFE0000)

/* Internal Flash page size */
#define INTERNAL_FLASH_PAGE_SIZE      ((uint32_t)0x400)       //1k Byte
#define INTERNAL_FLASH_SECTOR_SIZE    ((uint32_t)0x2000)      //4k Byte

#define USER_FIRMWARE_IMAGE_LOCATION CORE_FW_ADDRESS
#define FIRMWARE_IMAGE_SIZE           0x19000      //100K (firmware size)


#ifdef USE_SERIAL_FLASH
/* External Flash memory address where Factory programmed core firmware is located */
#define EXTERNAL_FLASH_FAC_ADDRESS  ((uint32_t)0x4000)
/* External Flash memory address where core firmware will be saved for backup/restore */
#define EXTERNAL_FLASH_BKP_ADDRESS  ((uint32_t)EXTERNAL_FLASH_FAC_ADDRESS)
/* External Flash memory address where OTA upgraded core firmware will be saved */
#define EXTERNAL_FLASH_OTA_ADDRESS  ((uint32_t)(EXTERNAL_FLASH_FAC_ADDRESS + FIRMWARE_IMAGE_SIZE))
#endif

#if FIRMWARE_IMAGE_SIZE > INTERNAL_FLASH_SIZE
#   error "FIRMWARE_IMAGE_SIZE too large to fit into internal flash"
#endif

/* Bootloader Flash regions that needs to be protected: 0x08000000 - 0x08004FFF */
#define BOOTLOADER_FLASH_PAGES      (OB_WRP_PAGES0TO3|OB_WRP_PAGES4TO7|OB_WRP_PAGES8TO11|OB_WRP_PAGES12TO15|OB_WRP_PAGES16TO19)


void FLASH_WriteProtection_Enable(uint32_t FLASH_WRP_Sectors);
void FLASH_WriteProtection_Disable(uint32_t FLASH_WRP_Sectors);
uint32_t FLASH_PagesMask(uint32_t imageSize, uint32_t pageSize);


#include "flash_access.h"

#ifdef __cplusplus
}
#endif


#endif  /*__FLASH_MAL_H*/
