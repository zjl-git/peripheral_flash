#include "flash_w25qxx.h"



void flash_w25qxx_init(void)
{
    flash_w25qxx_port_init();
}

void flash_w25qxx_write_enable(void)
{
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_WriteEnable);

    FLASH_CS_HIGH();
}

void flash_w25qxx_wait_write_end(void)
{
    uint8_t status = 0;
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_ReadStatusReg);
    do {
        status = flash_w251xx_port_read_byte();
    } while ((status & WIP_Flag) == true);

    FLASH_CS_HIGH();
}

void flash_w25qxx_erase_sector(uint32_t sector_addr)
{
    flash_w25qxx_write_enable();
    flash_w25qxx_wait_write_end();
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_SectorErase);
    flash_w25qxx_port_write_byte((sector_addr & 0xFF0000) >> 16);
    flash_w25qxx_port_write_byte((sector_addr& 0xFF00) >> 8);
    flash_w25qxx_port_write_byte(sector_addr & 0xFF);

    FLASH_CS_HIGH();
    flash_w25qxx_wait_write_end();
}

void flash_w25qxx_erase_bulk(void)
{
    flash_w25qxx_write_enable();
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_ChipErase);

    FLASH_CS_HIGH();
    flash_w25qxx_wait_write_end();
}

void flash_w25qxx_write_page(uint32_t write_addr, uint8_t *buffer, uint16_t len)
{
    flash_w25qxx_write_enable();
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_PageProgram);
    flash_w25qxx_port_write_byte((write_addr & 0xFF0000) >> 16);
    flash_w25qxx_port_write_byte((write_addr& 0xFF00) >> 8);
    flash_w25qxx_port_write_byte(write_addr & 0xFF);

    if (len > FLASH_PAGE_SIZE) {
        len = FLASH_PAGE_SIZE;
    }

    while (len--) {
        flash_w25qxx_port_write_byte(*buffer);
        buffer++;
    }

    FLASH_CS_HIGH();
    flash_w25qxx_wait_write_end();
}

void flash_w25qxx_write_buffer(uint32_t write_addr, uint8_t *buffer, uint16_t len)
{
    uint8_t page_num = 0, single_num = 0, addr = 0, count = 0, temp = 0;

    addr = write_addr % FLASH_PAGE_SIZE;
    count = FLASH_PAGE_SIZE - addr;
    page_num = len / FLASH_PAGE_SIZE;
    single_num = len % FLASH_PAGE_SIZE;

    if (addr == 0) {    /*write addr is page aligned*/
        if (page_num == 0) {
            flash_w25qxx_write_page(write_addr, buffer, len);
        } else {
            while (page_num--) {
                flash_w25qxx_write_page(write_addr, buffer, FLASH_PAGE_SIZE);
                write_addr += FLASH_PAGE_SIZE;
                buffer += FLASH_PAGE_SIZE;
            }
            flash_w25qxx_write_page(write_addr, buffer, single_num);
        }
    } else {
        if (page_num == 0) {
            if (single_num > count) {
                temp = single_num - count;
                flash_w25qxx_write_page(write_addr, buffer, count);
                write_addr += count;
                buffer += count;
                flash_w25qxx_write_page(write_addr, buffer, temp);
            } else {
                flash_w25qxx_write_page(write_addr, buffer, len);
            }
        } else {
            len -= count;
            page_num = len / FLASH_PAGE_SIZE;
            single_num = len % FLASH_PAGE_SIZE;

            flash_w25qxx_write_page(write_addr, buffer, count);
            write_addr += count;
            buffer += count;

            while (page_num--) {
                flash_w25qxx_write_page(write_addr, buffer, FLASH_PAGE_SIZE);
                write_addr += FLASH_PAGE_SIZE;
                buffer += FLASH_PAGE_SIZE;
            }

            if (single_num != 0) {
                flash_w25qxx_write_page(write_addr, buffer, single_num);
            }
        }
    }
}

void flash_w25qxx_read_buffer(uint32_t read_addr, uint8_t *buffer, uint16_t len)
{
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_ReadData);
    flash_w25qxx_port_write_byte((read_addr & 0xFF0000) >> 16);
    flash_w25qxx_port_write_byte((read_addr& 0xFF00) >> 8);
    flash_w25qxx_port_write_byte(read_addr & 0xFF);

    while (len--) {
        *buffer = flash_w251xx_port_read_byte();
        buffer++;
    }

    FLASH_CS_HIGH();
}

uint32_t flash_w25qxx_read_id(void)
{
    uint32_t id = 0, id0 = 0, id1 = 0, id2 = 0;
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_JedecDeviceID);
    id0 = flash_w251xx_port_read_byte();
    id1 = flash_w251xx_port_read_byte();
    id2 = flash_w251xx_port_read_byte();

    FLASH_CS_HIGH();
    id = (id0 << 16) | (id1 << 8) | id2;
    return id;
}

uint32_t flash_w25qxx_read_device_id(void)
{
    uint32_t id = 0;
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_DeviceID);
    flash_w25qxx_port_write_byte(Dummy_Byte);
    flash_w25qxx_port_write_byte(Dummy_Byte);
    flash_w25qxx_port_write_byte(Dummy_Byte);

    id = flash_w251xx_port_read_byte();

    FLASH_CS_HIGH();
    return id;
}

void flash_w25qxx_read_start_sequence(uint32_t read_addr)
{
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_ReadData);
    flash_w25qxx_port_write_byte((read_addr & 0xFF0000) >> 16);
    flash_w25qxx_port_write_byte((read_addr& 0xFF00) >> 8);
    flash_w25qxx_port_write_byte(read_addr & 0xFF);
}

void flash_w25qxx_read_end_sequence(void)
{
    FLASH_CS_HIGH();
}

uint8_t flash_w25qxx_read_byte(void)
{
    return flash_w251xx_port_read_byte();
}

void flash_w25qxx_power_down(void)
{
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_PowerDown);

    FLASH_CS_HIGH();
}

void flash_w25qxx_wake_up(void)
{
    FLASH_CS_LOW();

    flash_w25qxx_port_write_byte(W25X_ReleasePowerDown);

    FLASH_CS_HIGH();
}