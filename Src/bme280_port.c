#include "bme280_port.h"

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)intf_ptr;
    if(HAL_I2C_Mem_Read(hi2c, (BME280_I2C_ADDR_PRIM << 1), reg_addr, 1, reg_data, len, 100) == HAL_OK) return 0;
    return -1;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)intf_ptr;
    if(HAL_I2C_Mem_Write(hi2c, (BME280_I2C_ADDR_PRIM << 1), reg_addr, 1, (uint8_t *)reg_data, len, 100) == HAL_OK) return 0;
    return -1;
}

void user_delay_us(uint32_t period, void *intf_ptr) {
    uint32_t ms = period / 1000;
    if (ms == 0) ms = 1;
    HAL_Delay(ms);
}

int8_t bme280_initialize_stm32(struct bme280_dev *dev, I2C_HandleTypeDef *hi2c) {
    dev->intf_ptr = hi2c;
    dev->intf = BME280_I2C_INTF;
    dev->read = user_i2c_read;
    dev->write = user_i2c_write;
    dev->delay_us = user_delay_us;
    return bme280_init(dev);
}
