################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Drivers/bme280_driver.c \
../Src/Drivers/mpu6050_driver.c 

OBJS += \
./Src/Drivers/bme280_driver.o \
./Src/Drivers/mpu6050_driver.o 

C_DEPS += \
./Src/Drivers/bme280_driver.d \
./Src/Drivers/mpu6050_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Drivers/%.o Src/Drivers/%.su Src/Drivers/%.cyclo: ../Src/Drivers/%.c Src/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Inc -I"C:/STM/MultiSensor_Logger/Inc/Drivers" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-Drivers

clean-Src-2f-Drivers:
	-$(RM) ./Src/Drivers/bme280_driver.cyclo ./Src/Drivers/bme280_driver.d ./Src/Drivers/bme280_driver.o ./Src/Drivers/bme280_driver.su ./Src/Drivers/mpu6050_driver.cyclo ./Src/Drivers/mpu6050_driver.d ./Src/Drivers/mpu6050_driver.o ./Src/Drivers/mpu6050_driver.su

.PHONY: clean-Src-2f-Drivers

