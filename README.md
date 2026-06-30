# MultiSensor Logger (STM32 & BME280)

## Overview
This repository contains the firmware for a high-performance, real-time environmental data logging system. Designed around the STM32F411CEU6 microcontroller, the system interfaces with a Bosch Sensortec BME280 sensor to continuously monitor ambient physical properties (temperature, pressure, and humidity). The system bridges embedded electronic design with environmental physics, utilizing I2C for precise sensor configuration and data retrieval, and UART for real-time serial telemetry to a host PC.

## Key Features
* **Bare-Metal Hardware Control:** Utilizes STM32 HAL drivers for robust peripheral management.
* **Official Sensor API Integration:** Incorporates the latest standard Bosch BME280 API (v3.x) with custom hardware porting for I2C read/write and microsecond delays.
* **Optimized Build Structure:** Configured with a flattened, basic directory structure and filtered HAL libraries to ensure a lightweight, error-free compilation environment.
* **Floating-Point Serial Output:** Integrated `newlib-nano` standard library for precise floating-point `printf` telemetry over UART.

## Hardware Architecture
* **Microcontroller:** STM32F411CEU6 (ARM Cortex-M4, UFQFPN48 package)
* **Sensor:** Bosch BME280 (Temperature, Pressure, Humidity)
* **System Clock:** 25 MHz External HSE (Crystal/Ceramic Resonator) configured via internal PLL to run the system efficiently.

### Pin Configuration
| Peripheral | Pin  | Function | Electrical Details |
| :--- | :--- | :--- | :--- |
| **I2C1** | PB6 | SCL (Serial Clock) | 400 kHz Fast Mode |
| **I2C1** | PB7 | SDA (Serial Data) | 400 kHz Fast Mode |
| **USART2** | PA2 | TX (Transmit to PC) | 115200 bps, 8N1 |
| **USART2** | PA3 | RX (Receive) | 115200 bps, 8N1 |

## Software & Toolchain
* **Development Environment:** STM32CubeIDE 
* **Compiler:** GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`)
* **Sensor Configuration:** 
  * Pressure Oversampling: 16x
  * Temperature Oversampling: 2x
  * Humidity Oversampling: 1x
  * IIR Filter Coefficient: 16

## Data Telemetry Format
The system streams real-time data over the serial port once every second. Data can be captured using PuTTY, Tera Term, or a custom Python script.

**Format:**
`Temperature(°C), Pressure(hPa), Humidity(%)`

**Example Output:**
```text
System Booting... Hardware Initialized.
BME280 Initialized Successfully!
24.53,1013.25,45.12
24.54,1013.26,45.10
