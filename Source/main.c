/* Includes */
#include "main.h"
#include "uart_debug.h"
#include "tm1650.h"
#include "iwdg.h"
#include "input_scan.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "timers.h"
#include "event_groups.h"

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
#define Dispaly_Task_SIZE   256                 /* 任务堆栈大小 */
TaskHandle_t    Dispaly_Task_Handler;  /* 任务句柄 */

/* Input_Scan_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Input_Scan_Task_PRIO    3                   /* 任务优先级 */
#define Input_Scan_Task_SIZE    256                 /* 任务堆栈大小 */
TaskHandle_t    Input_Scan_Task_Handler;  /* 任务句柄 */

EventGroupHandle_t EventGroupHandler;       /* 事件标志组句柄 */

#define EVENTBIT_0      (1 << 0)            /* 事件位 */
#define EVENTBIT_1      (1 << 1)
#define EVENTBIT_ALL    (EVENTBIT_0 | EVENTBIT_1)

/* User create task */
static void start_task(void *pvParameters); /* 任务函数声明 */

int main(void)
{
    uart_debug_init();
    input_scan_init();
    TM1650_init();
//    IWDTInit();
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
    /* 创建事件标志组 */
    EventGroupHandler = xEventGroupCreate();
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
