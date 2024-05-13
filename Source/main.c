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
#include "Board.h"
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

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */

/* TASK1 任务 配置
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
void task1(void* pvParameters);             /* 任务函数声明 */
void task2(void *pvParameters);             /* 任务函数声明 */


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
    xTaskCreate((TaskFunction_t )task1,                 /* 任务函数 */
                (const char*    )"task1",               /* 任务名称 */
                (uint16_t       )TASK1_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )TASK1_PRIO,            /* 任务优先级 */
                (TaskHandle_t*  )&Task1Task_Handler);   /* 任务句柄 */

    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,                 /* 任务函数 */
                (const char*    )"task2",               /* 任务名称 */
                (uint16_t       )TASK2_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )TASK2_PRIO,            /* 任务优先级 */
                (TaskHandle_t*  )&Task2Task_Handler);   /* 任务句柄 */

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       Led toggle task
 * @param       pvParameters - passed into the task function as the function parameters
 * @retval      None
 */
void task1(void* pvParameters)
{
    while (1)
    {
        /* Toggle LED2 */
        APM_MINI_LEDToggle(LED2);
        /* Task blocking time Delay */
        vTaskDelay(500);
    }
}

/**
 * @brief       Led toggle task
 * @param       pvParameters - passed into the task function as the function parameters
 * @retval      None
 */
void task2(void* pvParameters)
{

    uint32_t        i               = 0;
    UBaseType_t     task_num        = 0;
    TaskStatus_t    *status_array   = NULL;
    TaskHandle_t    task_handle     = NULL;
    TaskStatus_t    *task_info      = NULL;
    eTaskState      task_state      = eInvalid;
    char            *task_state_str = NULL;
    char            *task_info_buf  = NULL;

    /* 第一步：函数uxTaskGetSystemState()的使用 */
    printf("/********第一步：函数uxTaskGetSystemState()的使用**********/\r\n");
    task_num = uxTaskGetNumberOfTasks();                            /* 获取系统任务数量 */
    status_array = mymalloc(SRAMIN, task_num * sizeof(TaskStatus_t));
    task_num = uxTaskGetSystemState((TaskStatus_t* )status_array,   /* 任务状态信息buffer */
                                    (UBaseType_t   )task_num,       /* buffer大小 */
                                    (uint32_t*     )NULL);          /* 不获取任务运行时间信息 */
    printf("任务名\t\t优先级\t\t任务编号\r\n");
    for (i=0; i<task_num; i++)
    {
        printf("%s\t%s%ld\t\t%ld\r\n",
                status_array[i].pcTaskName,
                strlen(status_array[i].pcTaskName) > 7 ? "": "\t",
                status_array[i].uxCurrentPriority,
                status_array[i].xTaskNumber);
    }
    myfree(SRAMIN, status_array);
    printf("/**************************结束***************************/\r\n");
    while(!(g_usart_rx_sta & 0x8000));
    g_usart_rx_sta = 0;

    /* 第二步：函数vTaskGetInfo()的使用 */
    printf("/************第二步：函数vTaskGetInfo()的使用**************/\r\n");
    task_info = mymalloc(SRAMIN, sizeof(TaskStatus_t));
    task_handle = xTaskGetHandle("task1");                          /* 获取任务句柄 */
    vTaskGetInfo((TaskHandle_t  )task_handle,                       /* 任务句柄 */
                 (TaskStatus_t* )task_info,                         /* 任务信息buffer */
                 (BaseType_t    )pdTRUE,                            /* 允许统计任务堆栈历史最小值 */
                 (eTaskState    )eInvalid);                         /* 获取任务运行状态 */
    printf("任务名:\t\t\t%s\r\n", task_info->pcTaskName);
    printf("任务编号:\t\t%ld\r\n", task_info->xTaskNumber);
    printf("任务壮态:\t\t%d\r\n", task_info->eCurrentState);
    printf("任务当前优先级:\t\t%ld\r\n", task_info->uxCurrentPriority);
    printf("任务基优先级:\t\t%ld\r\n", task_info->uxBasePriority);
    printf("任务堆栈基地址:\t\t0x%p\r\n", task_info->pxStackBase);
    printf("任务堆栈历史剩余最小值:\t%d\r\n", task_info->usStackHighWaterMark);
    myfree(SRAMIN, task_info);
    printf("/**************************结束***************************/\r\n");
    while(!(g_usart_rx_sta & 0x8000));
    g_usart_rx_sta = 0;
    /* 第三步：函数eTaskGetState()的使用  */
    printf("/***********第三步：函数eTaskGetState()的使用*************/\r\n");
    task_state_str = mymalloc(SRAMIN, 10);
    task_handle = xTaskGetHandle("task1");
    task_state = eTaskGetState(task_handle);                        /* 获取任务运行状态 */
    sprintf(task_state_str, task_state == eRunning ? "Runing" :
                            task_state == eReady ? "Ready" :
                            task_state == eBlocked ? "Blocked" :
                            task_state == eSuspended ? "Suspended" :
                            task_state == eDeleted ? "Deleted" :
                            task_state == eInvalid ? "Invalid" :
                                                     "");
    printf("任务状态值: %d，对应状态为: %s\r\n", task_state, task_state_str);
    myfree(SRAMIN, task_state_str);
    printf("/**************************结束***************************/\r\n");
    printf("按下KEY0键继续!\r\n\r\n\r\n");
    while(!(g_usart_rx_sta & 0x8000));
    g_usart_rx_sta = 0;

    /* 第四步：函数vTaskList()的使用 */
    printf("/*************第四步：函数vTaskList()的使用*************/\r\n");
    task_info_buf = mymalloc(SRAMIN, 500);
    vTaskList(task_info_buf);                                       /* 获取所有任务的信息 */
    printf("任务名\t\t状态\t优先级\t剩余栈\t任务序号\r\n");
    printf("%s\r\n", task_info_buf);
    myfree(SRAMIN, task_info_buf);
    printf("/************************实验结束***************************/\r\n");

    while (1)
    {
        vTaskDelay(10);
    }
}
