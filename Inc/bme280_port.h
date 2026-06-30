#ifndef BME280_PORT_H
#define BME280_PORT_H

#include "stm32f4xx_hal.h"
#include "bme280.h"

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);
int8_t bme280_initialize_stm32(struct bme280_dev *dev, I2C_HandleTypeDef *hi2c);

#endif
