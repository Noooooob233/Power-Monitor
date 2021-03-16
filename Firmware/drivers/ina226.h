#ifndef __INA226_H
#define __INA226_H

#include "stm32f1xx_hal.h"

typedef union _ina226_config_reg_t
{
    uint16_t raw;
    struct _bit
    {
        uint16_t mode : 3;
        uint16_t vshct : 3;
        uint16_t vbusct : 3;
        uint16_t avg : 3;
        uint16_t : 3;
        uint16_t rst : 1;
    } bit;
} ina226_config_reg_t;

void ina226_init(uint8_t addr);

void ina226_set_config_reg(uint8_t addr, ina226_config_reg_t *config);
void ina226_set_cal_reg(uint8_t addr, uint16_t cal_reg);

float ina226_read_vbus(uint8_t addr);
float ina226_read_current(uint8_t addr);
float ina226_read_power(uint8_t addr);

#endif
