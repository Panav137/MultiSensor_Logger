#ifndef BME280_DRIVER_H
#define BME280_DRIVER_H
#include "stm32f4xx_hal.h"
HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c);
void BME280_Read(float *temperature, float *humidity, float *pressure);
#endif
