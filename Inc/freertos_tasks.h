#ifndef FREERTOS_TASKS_H
#define FREERTOS_TASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sensor_types.h"

/* Queue and mutex handles — defined in freertos_tasks.c, extern here */
extern QueueHandle_t xSensorQueue;
extern QueueHandle_t xUARTQueue;
extern QueueHandle_t xHealthQueue;
extern SemaphoreHandle_t xSDMutex;

/* Task functions */
void SensorTask(void *pvParams);
void LoggerTask(void *pvParams);
void UARTTask(void *pvParams);
void WatchdogTask(void *pvParams);
void DisplayTask(void *pvParams);

/* Call once from main() before vTaskStartScheduler() */
void RTOS_CreateAll(void);

#endif
