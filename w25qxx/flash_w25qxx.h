#ifndef _FLASH_W25QXX__
#define _FLASH_W25QXX__

#ifdef __cplusplus
extern "C"
{
#endif

#include "flash_w25qxx_port.h"
#include "flash_w25qxx_config.h"

#if defined(W25Q128)
#define FLASH_BLOCK_NUM             256
#elif defined(W25Q64)
#define FLASH_BLOCK_NUM             128
#elif defined(W25Q32)
#define FLASH_BLOCK_NUM             64
#elif defined(W25Q16)
#define FLASH_BLOCK_NUM             32
#else
#define FLASH_BLOCK_NUM             0
#endif

#define FLASH_PAGE_NUM              16 * FLASH_SECTOR_NUM    
#define FLASH_SECTOR_NUM            16 * FLASH_BLOCK_NUM

#define FLASH_PAGE_SIZE             256
#define FLASH_SECTOR_SIZE           16 * FLASH_PAGE_SIZE
#define FLASH_BLOCK_SIZE            16 * FLASH_SECTOR_SIZE

#define FLASH_CS_LOW()              flash_w25qxx_port_cs_low()
#define FLASH_CS_HIGH()             flash_w25qxx_port_cs_high()

#define W25X_WriteEnable            0x06
#define W25X_WriteDisable           0x04
#define W25X_ReadStatusReg          0x05
#define W25X_WriteStatusReg         0x01
#define W25X_ReadData               0x03
#define W25X_FastReadData           0x0B
#define W25X_FastReadDual           0x3B
#define W25X_PageProgram            0x02
#define W25X_BlockErase             0xD8
#define W25X_SectorErase            0x20
#define W25X_ChipErase              0xC7
#define W25X_PowerDown              0xB9
#define W25X_ReleasePowerDown       0xAB
#define W25X_DeviceID               0xAB
#define W25X_ManufactDeviceID       0x90
#define W25X_JedecDeviceID          0x9F

#define Dummy_Byte                  0xFF
#define WIP_Flag                    0x01  /* Write In Progress (WIP) flag */

#ifdef __cplusplus
}
#endif

#endif
