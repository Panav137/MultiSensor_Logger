#include "mpu6050_driver.h"

#define MPU6050_ADDR      (0x68 << 1)
#define MPU_REG_PWR_MGMT  0x6B
#define MPU_REG_SMPLRT    0x19
#define MPU_REG_CONFIG    0x1A
#define MPU_REG_GYRO_CFG  0x1B
#define MPU_REG_ACCEL_CFG 0x1C
#define MPU_REG_INT_EN    0x38
#define MPU_REG_ACCEL_OUT 0x3B
#define MPU_REG_WHO_AM_I  0x75

static I2C_HandleTypeDef *_hi2c;

static HAL_StatusTypeDef mpu_write(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    return HAL_I2C_Master_Transmit(_hi2c, MPU6050_ADDR, buf, 2, 100);
}
static HAL_StatusTypeDef mpu_read(uint8_t reg, uint8_t *data, uint16_t len) {
    HAL_I2C_Master_Transmit(_hi2c, MPU6050_ADDR, &reg, 1, 100);
    return HAL_I2C_Master_Receive(_hi2c, MPU6050_ADDR, data, len, 100);
}

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    _hi2c = hi2c;
    uint8_t who;
    mpu_read(MPU_REG_WHO_AM_I, &who, 1);
    if (who != 0x68) return HAL_ERROR;
    mpu_write(MPU_REG_PWR_MGMT,  0x00);  // wake up
    mpu_write(MPU_REG_SMPLRT,    0x07);  // 1 kHz / (7+1) = 125 Hz
    mpu_write(MPU_REG_CONFIG,    0x00);
    mpu_write(MPU_REG_GYRO_CFG,  0x00);  // ±250 °/s
    mpu_write(MPU_REG_ACCEL_CFG, 0x00);  // ±2g
    mpu_write(MPU_REG_INT_EN,    0x01);  // data-ready interrupt
    return HAL_OK;
}

void MPU6050_Read(mpu6050_data_t *data) {
    uint8_t raw[14];
    mpu_read(MPU_REG_ACCEL_OUT, raw, 14);
    data->accel_x = (int16_t)(raw[0]<<8|raw[1]);
    data->accel_y = (int16_t)(raw[2]<<8|raw[3]);
    data->accel_z = (int16_t)(raw[4]<<8|raw[5]);
    data->gyro_x  = (int16_t)(raw[8]<<8|raw[9]);
    data->gyro_y  = (int16_t)(raw[10]<<8|raw[11]);
    data->gyro_z  = (int16_t)(raw[12]<<8|raw[13]);
}
