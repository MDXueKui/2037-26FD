#include "input_scan.h"
#include "core_cm0plus.h"
#include "FreeRTOS.h"
#include "event_groups.h"

void input_scan_init(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(ADD_GPIO_CLK | SET_GPIO_CLK |
                             rain_input_GPIO_CLK | up2_input_GPIO_CLK);
    /* ADD GPIO configuration */
    gpioConfig.pin = ADD_PIN;
    gpioConfig.mode = GPIO_MODE_IN;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(ADD_GPIO_PORT, &gpioConfig);
    /* SBU GPIO configuration */
    gpioConfig.pin = SBU_PIN;
    GPIO_Config(SBU_GPIO_PORT, &gpioConfig);
    /* RESET_KEY GPIO configuration */
    gpioConfig.pin = RESET_KEY_PIN;
    GPIO_Config(RESET_KEY_PORT, &gpioConfig);
    /* SET GPIO configuration */
    gpioConfig.pin = SET_PIN;
    GPIO_Config(SET_GPIO_PORT, &gpioConfig);
}

/**
 * @description: 输入扫描检测，低电平触发
 * @return {*}
 */
uint8_t input_scan(void)
{
    static uint8_t key_up = 0;
    uint8_t keyval = 0; /* 键值 */

    if (key_up && (ADD_KEY == 0 || SBU_KEY == 0 || RESET_KEY == 0)) /* 如果有按键按下 */
    {
        vTaskDelay(10); /* 延时10ms消抖 */
        key_up = 0;

        if (ADD_KEY == 0 && SBU_KEY == 0)
        {
            keyval = SET_PRES;
        }
        else if (ADD_KEY == 0)
        {
            keyval = ADD_PRES;
        }
        else if (SBU_KEY == 0)
        {
            keyval = SBU_PRES;
        }
        else if (RESET_KEY == 0)
        {
            keyval = RESET_KEY == 0;
        }
    }
    else if (ADD_KEY == 1 && SBU_KEY == 1 && RESET_KEY == 1) /* 无任何键按下继续等待 */
    {
        key_up = 1;
    }

    return keyval; /* 返回对应键值 */
}

TimerHandle_t Timer1Timer_Handler; /* 定时器1句柄 */

/**
 * @brief       Timer1超时回调函数
 * @param       xTimer : 传入参数(未用到)
 * @retval      无
 */
void Timer1Callback(TimerHandle_t xTimer)
{
    printf("test\r\n");
    // count++;
    // printf("count = %d\n", count);
    // if (count >= 5)
    // {
    //     xTimerStop((TimerHandle_t)Timer1Timer_Handler, /* 待停止的定时器句柄 */
    //                (TickType_t)portMAX_DELAY);         /* 等待系统停止定时器的最大时间 */
    // }
}

extern EventGroupHandle_t EventGroupHandler;

void Input_Scan_Task(void *parameter)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 定时器1创建为周期定时器 */
    Timer1Timer_Handler = xTimerCreate((const char*  )"Timer1",                 /* 定时器名 */
                                      (TickType_t   )1000,                      /* 定时器超时时间 */
                                      (UBaseType_t  )pdTRUE,                    /* 周期定时器 */
                                      (void*        )1,                         /* 定时器ID */
                                      (TimerCallbackFunction_t)Timer1Callback); /* 定时器回调函数 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
    BaseType_t timer_creat_state;
    printf("Input_Scan_Task start!\r\n");
    while (1)
    {
        uint8_t key_val = input_scan();

        switch (key_val)
        {
        case ADD_PRES:
            printf("ADD_PRES\r\n");
            xEventGroupSetBits((EventGroupHandle_t  )EventGroupHandler, /* 待操作的事件标志组句柄 */
                   (EventBits_t         )(1 << 0));       /* 待设置的bit位 */
            if (tube_display_buff[0] < 9)
            {
                tube_display_buff[0]++;
            }
            break;
        case SBU_PRES:
            printf("SBU_PRES\r\n");
            if (tube_display_buff[0] > 0)
            {
                tube_display_buff[0]--;
            }
            break;
        case RESET_KEY_PRES:
            break;
        case SET_PRES:
            if ((Timer1Timer_Handler != NULL))
            {
            printf("SET_PRES\n");
            timer_creat_state = xTimerStart((TimerHandle_t)Timer1Timer_Handler, /* 待启动的定时器句柄 */
                        (TickType_t)portMAX_DELAY);         /* 等待系统启动定时器的最大时间 */
            }
            printf("timer_creat_state = %d\n", timer_creat_state);
            break;
        default:
            break;
        }
        vTaskDelay(10);
    }
}


