#ifndef MPU6050_DRIVER_H
#define MPU6050_DRIVER_H
#include "stm32f4xx_hal.h"
typedef struct { int16_t accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z; } mpu6050_data_t;
HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_Read(mpu6050_data_t *data);
#endif
