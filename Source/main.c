/*!
 * @file       main.c
 *
 * @brief      Main program body
 *
 * @version    V1.0.0
 *
 * @date       2022-09-20
 *
 * @attention
 *
 *  Copyright (C) 2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "main.h"
#include "uart_debug.h"
#include "tm1650.h"
#include "input_scan.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "malloc.h"
#include "timers.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */

/* Dispaly_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Dispaly_Task_PRIO   2                   /* 任务优先级 */
#define Dispaly_Task_SIZE   32                 /* 任务堆栈大小 */
TaskHandle_t    Dispaly_Task_Handler;  /* 任务句柄 */

/* Input_Scan_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Input_Scan_Task_PRIO    3                   /* 任务优先级 */
#define Input_Scan_Task_SIZE    32                 /* 任务堆栈大小 */
TaskHandle_t    Input_Scan_Task_Handler;  /* 任务句柄 */

/* User create task */
static void start_task(void *pvParameters); /* 任务函数声明 */

int main(void)
{
    uart_debug_init();
    TM1650_init();
    input_scan_init();

    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    /* Startup FreeRTOS */
    vTaskStartScheduler();

    while (1);
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )Dispaly_Task,                 /* 任务函数 */
                (const char*    )"Dispaly_Task",               /* 任务名称 */
                (uint16_t       )Dispaly_Task_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )Dispaly_Task_PRIO,            /* 任务优先级 */
                (TaskHandle_t*  )&Dispaly_Task_Handler);   /* 任务句柄 */

    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )Input_Scan_Task,                 /* 任务函数 */
                (const char*    )"Input_Scan_Task",               /* 任务名称 */
                (uint16_t       )Input_Scan_Task_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )Input_Scan_Task_PRIO,            /* 任务优先级 */
                (TaskHandle_t*  )&Input_Scan_Task_Handler);   /* 任务句柄 */           
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}
