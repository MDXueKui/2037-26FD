#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f0xx_fmc.h"
#include "uart_debug.h"
#include "rf_receive.h"

/* flash 起始地址 */
#define apm32_flash_base 0x08000000 /* apm32 flash 起始地址 */
#define apm32_flash_size 0xFFFF    /* apm32 flash 总大小 */

/* apm32flash 扇区大小 */
#if apm32_flash_size < 256 * 1024
#define apm_sector_size 1024 /* 容量小于 256K 的 F103, 扇区大小为 1K 字节 */
#else
#define apm_sector_size 2048 /* 容量大于等于 256K 的 F103, 扇区大小为 2K 字节 */
#endif

/* apm32flash 预留做eeprom大小 */
#define flash_eeprom_base 0x0800FC00 /* 预留区域的起始地址 */
#define flash_eeprom_size 1024 /* 预留区域的大小 */

void flash_write_word(uint32_t addr, uint32_t data);
void flash_write_halfword(uint32_t addr, uint16_t data);
uint32_t flash_read_word(uint32_t faddr);
uint16_t flash_read_halfword(uint32_t faddr);

#ifdef __cplusplus
}
#endif

#endif
