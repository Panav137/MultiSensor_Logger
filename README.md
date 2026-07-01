# STM32 Multi-Sensor RTOS Logger 🚀

A highly robust, multi-threaded embedded diagnostic platform built on the STM32F411 (Black Pill). This system continuously monitors environmental data and 6-axis motion, logs it securely to an SD card, and streams real-time telemetry to a Python-based desktop dashboard.

This project demonstrates advanced embedded systems concepts including Real-Time Operating Systems (FreeRTOS), Direct Memory Access (DMA), bare-metal peripheral drivers, and hardware watchdog timers.

## ⚙️ Hardware Architecture

* **Microcontroller:** STM32F411CEUx (Black Pill) running at 100 MHz
* **Environment Sensor:** BME280 (Temperature, Humidity, Pressure) via `I2C1`
* **IMU Sensor:** MPU-6050 (3-Axis Accelerometer, 3-Axis Gyroscope) via `I2C2`
* **Storage:** MicroSD Card Module via `SPI1`
* **Telemetry:** Serial transmission via `USART2` (115200 Baud)

## 🧠 Software Architecture (FreeRTOS)

The firmware abandons the traditional `while(1)` super-loop in favor of a deterministic, preemptive RTOS architecture. It utilizes CMSIS-v2 FreeRTOS with 5 independent tasks:

1. **Sensor Task:** Polls the BME280 and MPU6050 using custom, lightweight bare-metal drivers. Calculates a CRC32 hash for data integrity and pushes the payload to RTOS queues.
2. **Logger Task:** Receives queued data and writes it as a CSV payload to the SD Card using the FatFS middleware. Protected by Mutexes to prevent file corruption.
3. **UART Task:** Formats sensor data into JSON strings and transmits them to the host PC using **non-blocking DMA**.
4. **Watchdog Task:** Acts as a system health monitor. It expects periodic "health tokens" from all other tasks via a queue. If a task hangs, the hardware Independent Watchdog (IWDG) resets the MCU.
5. **Display Task:** (Expandable) Reserved for OLED SSD1306 updates.

## 📁 Repository Structure

```text
├── Core/
│   ├── Inc/
│   │   ├── Drivers/        # Custom bare-metal header files (bme280, mpu6050)
│   │   ├── freertos_tasks.h
│   │   └── sensor_types.h  # Shared data structures
│   └── Src/
│       ├── Drivers/        # Custom bare-metal C files
│       ├── freertos_tasks.c
│       └── main.c          # Hardware init and RTOS startup
├── Dashboard/
│   ├── dashboard.py        # Real-time Matplotlib UI
│   └── requirements.txt    # Python dependencies
└── MultiSensor_Logger.ioc  # STM32CubeMX Configuration
