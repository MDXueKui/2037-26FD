/* Includes */
#include "main.h"
#include "uart_debug.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "malloc.h"

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
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */

/* Input_Scan_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */

List_t                  TestList;           /* 定义测试列表 */
ListItem_t              ListItem1;          /* 定义测试列表项1 */
ListItem_t              ListItem2;          /* 定义测试列表项2 */
ListItem_t              ListItem3;          /* 定义测试列表项3 */

/* User create task */
static void start_task(void *pvParameters); /* 任务函数声明 */

int main(void)
{
    uart_debug_init();
    APM_MINI_LEDInit(LED2);
    my_mem_init(SRAMIN);

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
                (UBaseType_t    )TASK2_PRIO,            /* 任务优先级 */
                (TaskHandle_t*  )&Task2Task_Handler);   /* 任务句柄 */

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}
