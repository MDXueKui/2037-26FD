/* Includes */
#include "main.h"
#include "uart_debug.h"
#include "tm1650.h"
#include "iwdg.h"
#include "input_scan.h"
#include "rf_receive.h"
#include "btim.h"
#include "gpio_output.h"

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
#define START_STK_SIZE  64                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */

/* Dispaly_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Dispaly_Task_PRIO   2                   /* 任务优先级 */
#define Dispaly_Task_SIZE   128                 /* 任务堆栈大小 */
TaskHandle_t    Dispaly_Task_Handler;  /* 任务句柄 */

/* Input_Scan_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Input_Scan_Task_PRIO    3                   /* 任务优先级 */
#define Input_Scan_Task_SIZE    200                 /* 任务堆栈大小 */
TaskHandle_t    Input_Scan_Task_Handler;  /* 任务句柄 */

/* Rf_Rec_Pro_Task 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define Rf_Rec_Pro_Task_PRIO    4                   /* 任务优先级 */
#define Rf_Rec_Pro_Task_SIZE    128                 /* 任务堆栈大小 */
TaskHandle_t    Rf_Rec_Pro_Task_Handler;  /* 任务句柄 */
/**
 * #define receive_eventbit_0      (1 << 0)
 * #define learn_eventbit_1        (1 << 1)
 * #define key_up_eventbit2        (1 << 2)
 * #define flashw_eventbit3        (1 << 3)
 * #define on_eventbit4            (1 << 4)
 * #define off_eventbit5           (1 << 5)
 * #define scintillation_eventbit6 (1 << 6)
 */
EventGroupHandle_t EventGroupHandler;       /* 事件标志组句柄 */

/* User create task */
static void start_task(void *pvParameters); /* 任务函数声明 */

int main(void)
{
    uart_debug_init();
    input_scan_init();
    buzzer_init();
    TM1650_init();
    rf_receive_Init();
    IWDTInit();
    gpio_output_init();
    btim_tim6_int_init(10-1, 720-1);

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
    BaseType_t xResult;
    taskENTER_CRITICAL();           /* 进入临界区 */
        /* 创建事件标志组 */
    EventGroupHandler = xEventGroupCreate();
    /* 创建任务1 */
    xResult = xTaskCreate((TaskFunction_t )Dispaly_Task,                 /* 任务函数 */
                         (const char*    )"Dispaly_Task",               /* 任务名称 */
                         (uint16_t       )Dispaly_Task_SIZE,        /* 任务堆栈大小 */
                         (void*          )NULL,                  /* 传入给任务函数的参数 */
                         (UBaseType_t    )Dispaly_Task_PRIO,            /* 任务优先级 */
                         (TaskHandle_t*  )&Dispaly_Task_Handler);   /* 任务句柄 */
    printf("Dispaly_Task create %s\r\n", xResult == pdPASS ? "pdTURE" : "pdFALSE");

    /* 创建任务2 */
    xResult = xTaskCreate((TaskFunction_t )Input_Scan_Task,                 /* 任务函数 */
                         (const char*    )"Input_Scan_Task",               /* 任务名称 */
                         (uint16_t       )Input_Scan_Task_SIZE,        /* 任务堆栈大小 */
                         (void*          )NULL,                  /* 传入给任务函数的参数 */
                         (UBaseType_t    )Input_Scan_Task_PRIO,            /* 任务优先级 */
                         (TaskHandle_t*  )&Input_Scan_Task_Handler);   /* 任务句柄 */
    printf("Input_Scan_Task create %s\r\n", xResult == pdPASS ? "pdTURE" : "pdFALSE");
    /* 创建任务3 */
    xResult = xTaskCreate((TaskFunction_t )Rf_Rec_Pro_Task,                 /* 任务函数 */
                         (const char*    )"Rf_Rec_Pro_Task",               /* 任务名称 */
                         (uint16_t       )Rf_Rec_Pro_Task_SIZE,        /* 任务堆栈大小 */
                         (void*          )NULL,                  /* 传入给任务函数的参数 */
                         (UBaseType_t    )Rf_Rec_Pro_Task_PRIO,            /* 任务优先级 */
                         (TaskHandle_t*  )&Rf_Rec_Pro_Task_Handler);   /* 任务句柄 */
    printf("Rf_Rec_Pro_Task create %s\r\n", xResult == pdPASS ? "pdTURE" : "pdFALSE");

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

void vApplicationTickHook(void)
{
    if (get_scintillation_eventbit6)
    {
        scintillation_time++;
        if (scintillation_time > 1000)
        {
            scintillation_time_count++;
            scintillation_time = 0;
        }
    }
    else if (get_on_eventbit4)
    {
        on_off_time_count++;
        if (on_off_time_count >= 1000)
        {
            temp_time_buff--;
            on_off_time_count = 0;
        }
        if (temp_time_buff > up_time_buff)
        {
            tube_display_buff[0] = 2;
            set_flashw_eventbit3(1);
            motor_stop();
            set_on_eventbit4(0);
        }
    }
    else if (get_off_eventbit5)
    {
        on_off_time_count++;
        if (on_off_time_count >= 1000)
        {
            temp_time_buff--;
            on_off_time_count = 0;
        }
        if (temp_time_buff > down_time_buff)
        {
            tube_display_buff[0] = 2;
            set_flashw_eventbit3(1);
            motor_stop();
            set_off_eventbit5(0);
        }
    }
    else if (get_fire_eventbit7 && (tube_display_buff[0] != 6) &&
             (fire_time_buff != 0))
    {
        on_off_time_count++;
        if (on_off_time_count >= 1000)
        {
            temp_time_buff--;
            on_off_time_count = 0;
        }
        if (temp_time_buff > fire_time_buff)
        {
            temp_time_buff = 0;
            motor_stop();
        }
    }
    else if (get_aging_eventbit8)
    {
        aging_time_count++;
        if (aging_time_count >= 1000)
        {
            temp_time_buff--;
            aging_time_count = 0;
        }
        if (temp_time_buff > aging_time_buff)
        {
            switch (aging_mode)
            {
            case 0:
                motor_up();
                aging_mode++;
                break;
            case 1:
                motor_stop();
                aging_mode++;
                break;
            case 2:
                motor_down();
                aging_mode++;
                break;
            case 3:
                motor_stop();
                aging_mode = 0;
                break;
            default:
                break;
            }
            temp_time_buff = aging_time_buff;
        }
    }
    else if (get_loaded_eventbit9)
    {
        on_off_time_count++;
        if (on_off_time_count >= 1000)
        {
            temp_time_buff--;
            on_off_time_count = 0;
        }
        if (temp_time_buff > down_time_buff)
        {
            motor_stop();
            set_loaded_eventbit9(0);
        }
    }
}
