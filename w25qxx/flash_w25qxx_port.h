#ifndef _FLASH_W25QXX_PORT__
#define _FLASH_W25QXX_PORT__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

void flash_w25qxx_port_init(void);

void flash_w25qxx_port_cs_low(void);

void flash_w25qxx_port_cs_high(void);

void flash_w25qxx_port_write_byte(uint8_t data);

uint8_t flash_w251xx_port_read_byte(void);

#ifdef __cplusplus
}
#endif

#endif
