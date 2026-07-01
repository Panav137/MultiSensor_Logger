#ifndef SENSOR_TYPES_H
#define SENSOR_TYPES_H

#include <stdint.h>

typedef struct {
    float temperature;   // °C
    float humidity;      // %RH
    float pressure;      // hPa
    int16_t accel_x;     // raw ADC
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    uint32_t timestamp;  // HAL_GetTick() ms
    uint32_t crc;        // CRC32 of above fields
} sensor_data_t;

typedef struct {
    uint8_t task_id;
} health_token_t;

#define TASK_ID_SENSOR   0x01
#define TASK_ID_LOGGER   0x02
#define TASK_ID_UART     0x03
#define TASK_ID_DISPLAY  0x04

#endif
