#include "bme280_driver.h"

#define BME280_ADDR        (0x76 << 1)
#define BME280_REG_ID      0xD0
#define BME280_REG_RESET   0xE0
#define BME280_REG_CTRL_HUM  0xF2
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_CONFIG    0xF5
#define BME280_REG_PRESS_MSB 0xF7

static I2C_HandleTypeDef *_hi2c;
static int32_t t_fine;

/* Calibration data */
static uint16_t dig_T1;
static int16_t  dig_T2, dig_T3;
static uint16_t dig_P1;
static int16_t  dig_P2,dig_P3,dig_P4,dig_P5,dig_P6,dig_P7,dig_P8,dig_P9;
static uint8_t  dig_H1, dig_H3;
static int16_t  dig_H2, dig_H4, dig_H5;
static int8_t   dig_H6;

static HAL_StatusTypeDef bme280_write_reg(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    return HAL_I2C_Master_Transmit(_hi2c, BME280_ADDR, buf, 2, 100);
}

static HAL_StatusTypeDef bme280_read_regs(uint8_t reg, uint8_t *data, uint16_t len) {
    HAL_I2C_Master_Transmit(_hi2c, BME280_ADDR, &reg, 1, 100);
    return HAL_I2C_Master_Receive(_hi2c, BME280_ADDR, data, len, 100);
}

static void bme280_read_calibration(void) {
    uint8_t buf[26];
    bme280_read_regs(0x88, buf, 26);
    dig_T1 = (buf[1]<<8)|buf[0];
    dig_T2 = (buf[3]<<8)|buf[2];
    dig_T3 = (buf[5]<<8)|buf[4];
    dig_P1 = (buf[7]<<8)|buf[6];
    dig_P2 = (buf[9]<<8)|buf[8];
    dig_P3 = (buf[11]<<8)|buf[10];
    dig_P4 = (buf[13]<<8)|buf[12];
    dig_P5 = (buf[15]<<8)|buf[14];
    dig_P6 = (buf[17]<<8)|buf[16];
    dig_P7 = (buf[19]<<8)|buf[18];
    dig_P8 = (buf[21]<<8)|buf[20];
    dig_P9 = (buf[23]<<8)|buf[22];
    dig_H1 = buf[25];
    uint8_t hbuf[7];
    bme280_read_regs(0xE1, hbuf, 7);
    dig_H2 = (hbuf[1]<<8)|hbuf[0];
    dig_H3 = hbuf[2];
    dig_H4 = (hbuf[3]<<4)|(hbuf[4]&0x0F);
    dig_H5 = (hbuf[5]<<4)|(hbuf[4]>>4);
    dig_H6 = hbuf[6];
}

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c) {
    _hi2c = hi2c;
    uint8_t id;
    bme280_read_regs(BME280_REG_ID, &id, 1);
    if (id != 0x60) return HAL_ERROR;   // wrong chip
    bme280_write_reg(BME280_REG_RESET, 0xB6);  // soft reset
    HAL_Delay(10);
    bme280_read_calibration();
    bme280_write_reg(BME280_REG_CTRL_HUM,  0x01);  // humidity oversampling x1
    bme280_write_reg(BME280_REG_CTRL_MEAS, 0x27);  // temp/press os x1, normal mode
    bme280_write_reg(BME280_REG_CONFIG,    0xA0);  // 1000 ms standby
    return HAL_OK;
}

void BME280_Read(float *temperature, float *humidity, float *pressure) {
    uint8_t raw[8];
    bme280_read_regs(BME280_REG_PRESS_MSB, raw, 8);

    int32_t adc_P = ((int32_t)raw[0]<<12)|((int32_t)raw[1]<<4)|(raw[2]>>4);
    int32_t adc_T = ((int32_t)raw[3]<<12)|((int32_t)raw[4]<<4)|(raw[5]>>4);
    int32_t adc_H = ((int32_t)raw[6]<<8)|raw[7];

    /* Temperature compensation (from Bosch datasheet) */
    int32_t var1 = ((((adc_T>>3)-((int32_t)dig_T1<<1)))*((int32_t)dig_T2))>>11;
    int32_t var2 = (((((adc_T>>4)-((int32_t)dig_T1))*((adc_T>>4)-((int32_t)dig_T1)))>>12)*((int32_t)dig_T3))>>14;
    t_fine = var1 + var2;
    *temperature = (float)((t_fine*5+128)>>8) / 100.0f;

    /* Pressure compensation */
    int64_t p1 = ((int64_t)t_fine) - 128000;
    int64_t p2 = p1*p1*(int64_t)dig_P6;
    p2 += ((p1*(int64_t)dig_P5)<<17);
    p2 += (((int64_t)dig_P4)<<35);
    p1 = ((p1*p1*(int64_t)dig_P3)>>8)+((p1*(int64_t)dig_P2)<<12);
    p1 = (((((int64_t)1)<<47)+p1)*((int64_t)dig_P1))>>33;
    if (p1 == 0) { *pressure = 0; }
    else {
        int64_t p = 1048576 - adc_P;
        p = (((p<<31)-p2)*3125)/p1;
        p1 = (((int64_t)dig_P9)*(p>>13)*(p>>13))>>25;
        p2 = (((int64_t)dig_P8)*p)>>19;
        p = ((p+p1+p2)>>8)+(((int64_t)dig_P7)<<4);
        *pressure = (float)p / 25600.0f;
    }

    /* Humidity compensation */
    int32_t h = t_fine - 76800;
    h = (((((adc_H<<14)-(((int32_t)dig_H4)<<20)-(((int32_t)dig_H5)*h))+16384)>>15)*
         (((((((h*((int32_t)dig_H6))>>10)*(((h*((int32_t)dig_H3))>>11)+32768))>>10)+2097152)*
           ((int32_t)dig_H2)+8192)>>14));
    h = h - (((((h>>15)*(h>>15))>>7)*((int32_t)dig_H1))>>4);
    h = (h<0)?0:h;
    h = (h>419430400)?419430400:h;
    *humidity = (float)(h>>12) / 1024.0f;
}
