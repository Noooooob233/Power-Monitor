#ifndef _FLASH_H 
#define _FLASH_H

#include "stm32f1xx_hal.h"

void eeprom_init(void);
uint8_t eeprom_read(uint16_t vaddress, uint16_t *data);
void eeprom_write(uint16_t vaddress, uint16_t data);
uint32_t flash_erase(uint32_t address, uint32_t num);
void flash_write(uint32_t address, uint16_t *p, int len);
void flash_read(uint32_t address, uint16_t *p, int len);

#endif
