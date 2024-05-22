/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:06:37
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-21 09:59:48
 * @FilePath: \FreeRTOS\Source\gpio_output.c
 * @Description:
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "gpio_output.h"
#include "tm1650.h"
#include "buzzer.h"

#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"

extern EventGroupHandle_t EventGroupHandler;

void gpio_output_init(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(UPOUT1_GPIO_CLK | LED3_GPIO_CLK);
    gpioConfig.pin = UPOUT1_GPIO_PIN;
    gpioConfig.mode = GPIO_MODE_OUT;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(UPOUT1_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = DNOUT1_GPIO_PIN;
    GPIO_Config(DNOUT1_GPIO_PORT, &gpioConfig);

    // gpioConfig.pin = cascaded_outputA_GPIO_PIN;
    // GPIO_Config(cascaded_outputA_GPIO_PORT, &gpioConfig);

    // gpioConfig.pin = cascaded_outputB_GPIO_PIN;
    // GPIO_Config(cascaded_outputB_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = firefeedback_output_GPIO_PIN;
    GPIO_Config(firefeedback_output_PORT, &gpioConfig);

    // gpioConfig.pin = controlfeedback_GPIO_PIN;
    // GPIO_Config(controlfeedback_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = LED1_GPIO_PIN;
    GPIO_Config(LED1_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = LED2_GPIO_PIN;
    GPIO_Config(LED2_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = LED3_GPIO_PIN;
    GPIO_Config(LED3_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = LED4_GPIO_PIN;
    GPIO_Config(LED4_GPIO_PORT, &gpioConfig);

    LED1(0);
    LED2(0);
    LED3(0);
    LED4(0);
    UPOUT1(0);
    DNOUT1(0);
    // cascaded_outputA(0);
    // cascaded_outputB(0);
    firefeedback_output(0);
    // controlfeedback_output(0);
}

void motor_up(void)
{
    if(GPIO_ReadOutputBit(DNOUT1_GPIO_PORT, DNOUT1_GPIO_PIN))
    {
        vTaskDelay(500);
        LED3(1);
        UPOUT1(1);
        DNOUT1(0);
        // cascaded_outputA(1);
        // cascaded_outputB(0);
    }
    else
    {
        LED3(1);
        UPOUT1(1);
        DNOUT1(0);
        // cascaded_outputA(1);
        // cascaded_outputB(0);
    }
}

void motor_down(void)
{
    if(GPIO_ReadOutputBit(UPOUT1_GPIO_PORT, UPOUT1_GPIO_PIN))
    {
        vTaskDelay(500);
        LED3(1);
        UPOUT1(0);
        DNOUT1(1);
        // cascaded_outputA(0);
        // cascaded_outputB(1);
    }
    else
    {
        LED3(1);
        UPOUT1(0);
        DNOUT1(1);
        // cascaded_outputA(0);
        // cascaded_outputB(1);
    }
}

void motor_stop(void)
{
    LED3(0);
    UPOUT1(0);
    DNOUT1(0);
    set_loaded_eventbit9(0);
    // cascaded_outputA(0);
    // cascaded_outputB(0);
}

void stop_motor(void)
{
    set_on_eventbit4(0);
    set_off_eventbit5(0);
    tube_display_buff[0] = 2;
    set_flashw_eventbit3(1);
    motor_stop();
}
extern TaskHandle_t Dispaly_Task_Handler;
/**
 * @description: 电机上总控
 * @return {*}
 */
void motor_on_all(void)
{
    if((!get_on_eventbit4) && (!get_fire_eventbit7))
    {
        vTaskSuspend(Dispaly_Task_Handler);
        if(get_off_eventbit5)
        {
            set_off_eventbit5(0);
        }
        tube_display_buff[0] = 7;
        temp_time_buff = up_time_buff;
        set_flashw_eventbit3(1);
        motor_up();
        on_off_time_count = 0;
        set_on_eventbit4(1);
        vTaskResume(Dispaly_Task_Handler);
    }
}

/**
 * @description: 电机下总控
 * @return {*}
 */
void motro_off_all(void)
{
    if((!get_off_eventbit5) && (!get_fire_eventbit7))
    {
        vTaskSuspend(Dispaly_Task_Handler);
        if(get_on_eventbit4)
        {
            set_on_eventbit4(0);
        }
        tube_display_buff[0] = 8;
        temp_time_buff = down_time_buff;
        set_flashw_eventbit3(1);
        motor_down();
        on_off_time_count = 0;
        set_off_eventbit5(1);
        vTaskResume(Dispaly_Task_Handler);
    }
}

/**
 * @description: 消防处理函数，包括24V消防
 * @return {*}
 */
void fire_proc(void)
{
    vTaskSuspend(Dispaly_Task_Handler);
    tube_display_buff[0] = 5;
    set_flashw_eventbit3(1);
    if(ManualAuto_flag)
    {
        temp_time_buff = fire_time_buff;
        on_off_time_count = 0;
        BUZZER(1);
        LED2(1);
        firefeedback_output(1);
        vTaskResume(Dispaly_Task_Handler);
    }
    else
    {
        temp_time_buff = fire_time_buff;
        on_off_time_count = 0;
        firefeedback_output(1);
        motor_up();
        BUZZER(1);
        LED2(1);
        set_fire_eventbit7(1);
        vTaskResume(Dispaly_Task_Handler);
    }
}
