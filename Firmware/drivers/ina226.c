#include "ina226.h"
#include "iic.h"

extern I2C_HandleTypeDef hi2c1;

void ina226_init(uint8_t addr)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE(); //GPIOB时钟开启

    GPIO_InitStruct.Pin = GPIO_PIN_12; // AL
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    ina226_config_reg_t config;

    config.bit.mode = 0x07;
    config.bit.vshct = 0x04;
    config.bit.vbusct = 0x04;
    config.bit.avg = 0x00;
    config.bit.rst = 0x00;
    ina226_set_config_reg(addr, &config);

    ina226_set_cal_reg(addr, 0x800);
}

void ina226_set_config_reg(uint8_t addr, ina226_config_reg_t *config)
{
    uint8_t data[2];

    data[0] = config->raw >> 8;
    data[1] = config->raw & 0xFF;

    HAL_I2C_Mem_Write(&hi2c1, addr, 0x00, 1, data, 2, 1000);
}

void ina226_set_cal_reg(uint8_t addr, uint16_t cal_reg)
{
    uint8_t data[2];

    data[0] = cal_reg >> 8;
    data[1] = cal_reg & 0xFF;

    HAL_I2C_Mem_Write(&hi2c1, addr, 0x05, 1, data, 2, 1000);
}

uint16_t ina226_read_id(uint8_t addr)
{
    uint16_t ret = 0;
    uint8_t data[2] = {0};

    HAL_I2C_Mem_Read(&hi2c1, addr, 0xFF, 1, data, 2, 1000);

    ret = (data[0] << 8) | (data[1] & 0xFF);

    return ret;
}

// mV
float ina226_read_vbus(uint8_t addr)
{
    float ret = 0;
    uint8_t data[2] = {0};

    HAL_I2C_Mem_Read(&hi2c1, addr, 0x02, 1, data, 2, 1000);

    ret = ((data[0] << 8) | (data[1] & 0xFF)) * 1.25;

    return ret;
}

// mA
float ina226_read_current(uint8_t addr)
{
    float ret = 0;
    uint16_t data_u16 = 0;
    uint8_t data[2] = {0};

    HAL_I2C_Mem_Read(&hi2c1, addr, 0x04, 1, data, 2, 1000);

    data_u16 = ((data[0] << 8) | (data[1] & 0xFF));
    ret = *((int16_t *)&data_u16) / 4.0;

    return ret;
}

// mW
float ina226_read_power(uint8_t addr)
{
    float ret = 0;
    uint8_t data[2] = {0};

    HAL_I2C_Mem_Read(&hi2c1, addr, 0x03, 1, data, 2, 1000);

    ret = ((data[0] << 8) | (data[1] & 0xFF)) * 6.25;

    return ret;
}
