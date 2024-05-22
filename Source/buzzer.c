/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:03:42
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-21 15:33:02
 * @FilePath: \FreeRTOS\Source\buzzer.c
 * @Description: 蜂鸣器驱动程序
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "buzzer.h"

uint32_t buzzer_time_count = 0;

/**
 * @description: 蜂鸣器初始化函数
 * @return {*}
 */
void buzzer_init(void)
{
    TMR_TimeBase_T  timeBaseConfig;
    TMR_OCConfig_T  occonfig;
    GPIO_Config_T   gpioconfig;

    /* Enable Clock */
    RCM_EnableAHBPeriphClock(BUZZER_GPIO_CLK);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_TMR15);

    /*  Connect TMR15 to CH2 */
    GPIO_ConfigPinAF(GPIOA, GPIO_PIN_SOURCE_3, GPIO_AF_PIN0);
    gpioconfig.mode  = GPIO_MODE_AF;
    gpioconfig.outtype = GPIO_OUT_TYPE_PP;
    gpioconfig.pin   = BUZZER_GPIO_PIN;
    gpioconfig.pupd  = GPIO_PUPD_NO;
    gpioconfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(BUZZER_GPIO_PORT, &gpioconfig);

    /* Set clockDivision = 1 */
    timeBaseConfig.clockDivision =  TMR_CKD_DIV1;
    /* Up-counter */
    timeBaseConfig.counterMode =  TMR_COUNTER_MODE_UP;
    /* Set divider = 47 .So TMR1 clock freq ~= 48/(47+1) = 1MHZ */
    timeBaseConfig.div = 47;
    /* Set counter = 0xffff */
    timeBaseConfig.period = 499;
    /* Repetition counter = 0x0 */
    timeBaseConfig.repetitionCounter =  0;
    TMR_ConfigTimeBase(TMR15, &timeBaseConfig);

    /* PWM1 mode */
    occonfig.OC_Mode =  TMR_OC_MODE_PWM1;
    /* Enable CH1 ouput */
    occonfig.OC_OutputState  =  TMR_OUTPUT_STATE_ENABLE;
    /* CH2  polarity is high */
    occonfig.OC_Polarity  = TMR_OC_POLARITY_LOW;
    /* Set compare value */
    occonfig.Pulse = 250;
    TMR_OC2Config(TMR15, &occonfig);

    /*  Enable TMR1  */
    TMR_Enable(TMR15);
}

/**
 * @description: 蜂鸣器按下响三下
 * @return {*}
 */
void buzzer_quick_3s(void)
{
    BUZZER(0);
    uint8_t work_count = 0;
    while(work_count < 3)
    {
        BUZZER(1);
        vTaskDelay(100);
        BUZZER(0);
        vTaskDelay(100);
        work_count++;
    }
    BUZZER(0);
}

/**
 * @description: 蜂鸣器按下响一下
 * @return {*}
 */
void buzzer_quick_100ms(void)
{
    BUZZER(1);
    vTaskDelay(100);
    BUZZER(0);
}
