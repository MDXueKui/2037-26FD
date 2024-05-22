#include "input_scan.h"
//#include "core_cm0plus.h"
#include "tm1650.h"
#include "buzzer.h"
#include "rf_receive.h"
#include "gpio_output.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"

extern EventGroupHandle_t EventGroupHandler;
void reset_pres_fun(void);
uint8_t stop_time_count = 0;

#define key_up_eventbit2    (1 << 2)
#define set_key_up(x)       \
                            do{ x ? \
                            xEventGroupSetBits(EventGroupHandler, key_up_eventbit2) : \
                            xEventGroupClearBits(EventGroupHandler, key_up_eventbit2); \
                            }while(0)

#define get_key_up          (xEventGroupGetBits(EventGroupHandler) & key_up_eventbit2)

void input_scan_init(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(ADD_GPIO_CLK | rain_input_GPIO_CLK | \
                                OFF_GPIO_CLK | MOSI_GPIO_CLK);
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

    /* ON_PIN GPIO configuration */
    gpioConfig.pin = ON_PIN;
    GPIO_Config(ON_GPIO_PORT, &gpioConfig);

    /* OFF_PIN GPIO configuration */
    gpioConfig.pin = OFF_PIN;
    GPIO_Config(OFF_GPIO_PORT, &gpioConfig);

    /* fire_control_input_PIN GPIO configuration */
    gpioConfig.pin = fire_control_input_PIN;
    GPIO_Config(fire_control_input_GPIO_PORT, &gpioConfig);

    /* fire_control24v_input_PIN GPIO configuration */
    gpioConfig.pin = fire_control24v_input_PIN;
    GPIO_Config(fire_control24v_input_GPIO_PORT, &gpioConfig);

    /* cascaded24vR_input_PIN GPIO configuration */
    gpioConfig.pin = cascaded24vR_input_PIN;
    GPIO_Config(cascaded24vR_input_GPIO_PORT, &gpioConfig);

    /* cascaded24vL_input_PIN GPIO configuration */
    gpioConfig.pin = cascaded24vL_input_PIN;
    GPIO_Config(cascaded24vL_input_GPIO_PORT, &gpioConfig);

    /* rain_input_PIN GPIO configuration */
    gpioConfig.pin = rain_input_PIN;
    GPIO_Config(rain_input_GPIO_PORT, &gpioConfig);

    /* MOSI_PIN GPIO configuration */
    gpioConfig.pin = MOSI_PIN;
    GPIO_Config(MOSI_GPIO_PORT, &gpioConfig);
}

/**
 * @description: 输入扫描检测，低电平触发
 * @return {*}
 */
uint8_t input_scan(void)
{
    uint8_t keyval = 0; /* 键值 */

    if (!get_key_up && (ADD_KEY == 0 || SBU_KEY == 0 || RESET_KEY == 0 || \
        ON_KEY == 0 || OFF_KEY == 0 || fire_control_input_KEY == 0 || \
        fire_control24v_input_KEY == 0 || cascaded24vR_input_KEY == 0 || \
        cascaded24vL_input_KEY == 0 || rain_input_KEY == 0 || MOSI_KEY == 0)) /* 如果有按键按下 */
    {
        vTaskDelay(10); /* 延时10ms消抖 */
        set_key_up(1);
        if ((!ON_KEY) && (!OFF_KEY) && (!get_fire_eventbit7))
        {
            buzzer_quick_100ms();
            if ((tube_display_buff[0] == 6 ||
                 tube_display_buff[0] == 1) &&
                (!get_fire_eventbit7))
            {
                motor_stop();
            }
            else
            {
                stop_motor();
            }
        }
        else if (ADD_KEY == 0 && SBU_KEY == 0 && (!get_fire_eventbit7))
        {
            keyval = SET_PRES;
        }
        else if (ADD_KEY == 0 && (!get_fire_eventbit7))
        {
            keyval = ADD_PRES;
        }
        else if (SBU_KEY == 0 && (!get_fire_eventbit7))
        {
            keyval = SBU_PRES;
        }
        else if (RESET_KEY == 0 )
        {
            keyval = RESET_KEY_PRES;
        }
        else if (ON_KEY == 0 && (!get_fire_eventbit7) && (!get_lock_eventbit12))
        {
            keyval = ON_PRES;
        }
        else if (OFF_KEY == 0 && (!get_fire_eventbit7) && (!get_lock_eventbit12))
        {
            keyval = OFF_PRES;
        }
        else if (fire_control_input_KEY == 0 && (!get_fire_eventbit7))
        {
            keyval = fire_control_input_Triggering;
        }
        else if (fire_control24v_input_KEY == 0 && (!get_fire_eventbit7))
        {
            keyval = fire_control24v_input_Triggering;
        }
        else if (cascaded24vR_input_KEY == 0 && (!get_fire_eventbit7) && (!get_lock_eventbit12))
        {
            keyval = cascaded24vR_input_Triggering;
        }
        else if (cascaded24vL_input_KEY == 0 && (!get_fire_eventbit7) && (!get_lock_eventbit12))
        {
            keyval = cascaded24vL_input_Triggering;
        }
        else if (rain_input_KEY == 0 && (!get_fire_eventbit7) && (!get_lock_eventbit12))
        {
            keyval = rain_input_Triggering;
        }
        else if (MOSI_KEY == 0 && (!get_fire_eventbit7) && (!get_lock_eventbit12))
        {
            keyval = MOSI_Triggering;
        }
    }
    else if (ADD_KEY == 1 && SBU_KEY == 1 && RESET_KEY == 1 && \
            ON_KEY == 1 && OFF_KEY == 1 && fire_control_input_KEY == 1 && \
            fire_control24v_input_KEY == 1 && cascaded24vR_input_KEY == 1 && \
            cascaded24vL_input_KEY == 1 && rain_input_KEY == 1 && MOSI_KEY == 1) /* 无任何键按下继续等待 */
    {
        set_key_up(0);
        stop_time_count = 0;
        if(tube_display_buff[0] == 1 && \
                (g_out_data1 == 0x0f || g_out_data1 == 0))
        {
            motor_stop();
        }

        if(get_cascaded24vR_input_eventbit10 || get_cascaded24vL_input_eventbit11)
        {
            set_cascaded24vR_input_eventbit10(0);
            set_cascaded24vL_input_eventbit11(0);
            motor_stop();
        }
    }

    if ((!ON_KEY) && (!OFF_KEY))
    {
        vTaskDelay(500);
        if ((!ON_KEY) && (!OFF_KEY))
        {
            stop_time_count++;
            if(stop_time_count >= 10)
            {
                keyval = RESET_KEY_PRES;
                stop_time_count = 0;
            }
        }
    }

    return keyval; /* 返回对应键值 */
}

TimerHandle_t Timer1Timer_Handler; /* 定时器1句柄 */
uint8_t count = 0;
/**
 * @brief       Timer1超时回调函数
 * @param       xTimer : 传入参数(未用到)
 * @retval      无
 */
void Timer1Callback(TimerHandle_t xTimer)
{
    printf("count = %d\r\n", count);
    static uint8_t buzzer_state = 0;
    static uint8_t tick = 0;

    if(get_key_up)
    {
        count++;
    }
    else
    {
        count = 0;
        if(!get_learn_eventbit_1)
        {
            buzzer_state = 0;
            tick = 0;
            xTimerStop((TimerHandle_t)Timer1Timer_Handler, /* 待停止的定时器句柄 */
                       (TickType_t)portMAX_DELAY);         /* 等待系统停止定时器的最大时间 */
        }
    }

    if (count >= 10 && (ON_KEY && OFF_KEY))
    {
        set_learn_eventbit_1(1);
    }
    else if(count >= 20 && !ON_KEY && tube_display_buff[0] == 6)
    {
        count = 0;
        set_lock_eventbit12(1);
        buzzer_quick_3s();
        xTimerStop((TimerHandle_t)Timer1Timer_Handler, /* 待停止的定时器句柄 */
                       (TickType_t)portMAX_DELAY);         /* 等待系统停止定时器的最大时间 */
    }
    else if(count >= 20 && !OFF_KEY && tube_display_buff[0] == 6)
    {
        count = 0;
        set_lock_eventbit12(0);
        buzzer_quick_3s();
        xTimerStop((TimerHandle_t)Timer1Timer_Handler, /* 待停止的定时器句柄 */
                       (TickType_t)portMAX_DELAY);         /* 等待系统停止定时器的最大时间 */
    }

    if(get_learn_eventbit_1)
    {
        tick++;
        if(tick >= 20)
        {
            tick = 0;
            set_learn_eventbit_1(0);
            xTimerStop((TimerHandle_t)Timer1Timer_Handler, /* 待停止的定时器句柄 */
                       (TickType_t)portMAX_DELAY);         /* 等待系统停止定时器的最大时间 */
        }
        buzzer_state = !buzzer_state;
        BUZZER(buzzer_state);
    }

}

void add_pres_fun(void)
{
    if (!get_scintillation_eventbit6 && tube_display_buff[0] < 9)
    {
        tube_display_buff[0]++;
        set_flashw_eventbit3(1);
    }
    else if(get_scintillation_eventbit6)
    {
        scintillation_time_count = 0;
        switch(tube_display_buff[0])
        {
            case 3:
                up_time_buff++;
                break;
            case 4:
                down_time_buff++;
                break;
            case 5:
                fire_time_buff++;
                break;
            case 9:
                aging_time_buff++;
                break;
            default:
                break;
        }
    }
}

void sbu_pres_fun(void)
{
    if (!get_scintillation_eventbit6 && tube_display_buff[0] > 0)
    {
        tube_display_buff[0]--;
        set_flashw_eventbit3(1);
    }
    else if(get_scintillation_eventbit6)
    {
        scintillation_time_count = 0;
        switch(tube_display_buff[0])
        {
            case 3:
                if(up_time_buff != 0)
                {
                    up_time_buff--;
                }
                break;
            case 4:
                if(up_time_buff != 0)
                {
                    down_time_buff--;
                }
                break;
            case 5:
                if(up_time_buff != 0)
                {
                    fire_time_buff--;
                }
                break;
            case 9:
                if(up_time_buff != 0)
                {
                    aging_time_buff--;
                }
                break;
            default:
                break;
        }
    }
    /* 查询系统历史剩余最小内存 */
//    printf("FreeRTO总内存大小：%d 字节\r\n",configTOTAL_HEAP_SIZE);//打印FreeRTOS总内存大小（单位字节）
//    printf("当前剩余堆内存大小：%d 字节\r\n", xPortGetFreeHeapSize());//查询当前剩余堆内存大小
//    printf("历史剩余最小内存大小：%d 字节\r\n\r\n",xPortGetMinimumEverFreeHeapSize());//查询历史剩余最小内存大小
}

void reset_pres_fun(void)
{
    // char *task_info_buf  = NULL;
    // char *runtime_info = NULL;

    if(get_on_eventbit4 || get_off_eventbit5)
    {
        set_on_eventbit4(0);
        set_off_eventbit5(0);
        tube_display_buff[0] = 2;
        set_fire_eventbit7(0);
    }
    set_fire_eventbit7(0);
    firefeedback_output(0);
    LED2(0);
    if(tube_display_buff[0] != 6)
    {
        motor_stop();
    }

    //     /* 第四步：函数vTaskList()的使用 */
    // task_info_buf = pvPortMalloc(200);
    // vTaskList(task_info_buf);                                       /* 获取所有任务的信息 */
    // printf("任务名\t\t状态\t优先级\t剩余栈\t任务序号\r\n");
    // printf("%s\r\n", task_info_buf);
    // runtime_info = pvPortMalloc(200);
    // vTaskGetRunTimeStats(runtime_info); /* 获取任务运行时间信息 */
    // printf("任务名\t\t 运行时间\t 运行所占百分比\r\n");
    // printf("%s\r\n", runtime_info);
    // /* 释放内存 */
    // if (NULL != task_info_buf && NULL != runtime_info)
    // {
    //     vPortFree(task_info_buf);
    //     vPortFree(runtime_info);
    //     task_info_buf = NULL;
    //     runtime_info = NULL;
    // }
    // printf("/************************实验结束***************************/\r\n");
}

void set_pres_fun(void)
{
    xTimerStart((TimerHandle_t)Timer1Timer_Handler, /* 待启动的定时器句柄 */
                                                (TickType_t)portMAX_DELAY);         /* 等待系统启动定时器的最大时间 */
    if(!get_scintillation_eventbit6 && (tube_display_buff[0] == 3 || \
        tube_display_buff[0] == 4 || tube_display_buff[0] == 5 || \
        tube_display_buff[0] == 9))
    {
        scintillation_time_count = 0;
        set_scintillation_eventbit6(1);
        scintillation_time = 0;
    }
    else
    {
        scintillation_time_count = 0;
        set_scintillation_eventbit6(0);
        scintillation_time = 0;
        set_flashw_eventbit3(1);
    }
}

void on_pres_fun(void)
{
    switch (tube_display_buff[0])
    {
    case 1:
        motor_up();
        break;
    case 5:
        if (ManualAuto_flag &&
            (GPIO_ReadOutputBit(firefeedback_output_PORT, firefeedback_output_GPIO_PIN)))
        {
            motor_up();
            BUZZER(1);
            set_fire_eventbit7(1);
        }
        else
        {
            motor_on_all();
        }
        break;
    case 6:
        motor_up();
        break;
    default:
        motor_on_all();
        break;
    }
}

void off_pres_fun(void)
{
    switch (tube_display_buff[0])
    {
    case 1:
        motor_down();
        break;
    case 6:
        motor_down();
        break;
    default:
        motro_off_all();
        break;
    }
}

void fire_pres_fun(void)
{
    switch (tube_display_buff[0])
    {
    case 6:
        set_fire_eventbit7(1);
        motor_stop();
        LED2(1);
        BUZZER(1);
        firefeedback_output(1);
        vTaskDelay(1000);
        motor_up();
        break;
    default:
        fire_proc();
        break;
    }
}

void Input_Scan_Task(void *parameter)
{
    /* 定时器1创建为周期定时器 */
    Timer1Timer_Handler = xTimerCreate((const char *)"Timer1",                   /* 定时器名 */
                                       (TickType_t)500,                         /* 定时器超时时间 */
                                       (UBaseType_t)pdTRUE,                      /* 周期定时器 */
                                       (void *)1,                                /* 定时器ID */
                                       (TimerCallbackFunction_t)Timer1Callback); /* 定时器回调函数 */
    printf("Input_Scan_Task start!\r\n");
    while (1)
    {
        uint8_t key_val;
        key_val = input_scan();
        switch (key_val)
        {
        case ADD_PRES:
            buzzer_quick_100ms();
            add_pres_fun();
            break;
        case SBU_PRES:
            buzzer_quick_100ms();
            sbu_pres_fun();
            break;
        case RESET_KEY_PRES:
            buzzer_quick_100ms();
            reset_pres_fun();
            break;
        case SET_PRES:
            buzzer_quick_100ms();
            set_pres_fun();
            break;
        case ON_PRES:
            buzzer_quick_100ms();
            on_pres_fun();
            break;
        case OFF_PRES:
            buzzer_quick_100ms();
            off_pres_fun();
            break;
        case fire_control_input_Triggering:
            buzzer_quick_100ms();
            fire_pres_fun();
            break;
        case fire_control24v_input_Triggering:
            buzzer_quick_100ms();
            fire_pres_fun();
            break;
        case rain_input_Triggering:
            buzzer_quick_100ms();
            motro_off_all();
            break;
        case cascaded24vR_input_Triggering:
            buzzer_quick_100ms();
            set_cascaded24vR_input_eventbit10(1);
            motor_up();
            break;
        case cascaded24vL_input_Triggering:
            buzzer_quick_100ms();
            set_cascaded24vL_input_eventbit11(1);
            motor_down();
            break;
        case MOSI_Triggering:
            buzzer_quick_100ms();
            ManualAuto_flag = !ManualAuto_flag;
            set_flashw_eventbit3(1);
            break;
        default:
            break;
        }
        vTaskDelay(10);
    }
}
