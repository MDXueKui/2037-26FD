/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:03:42
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-04-28 09:07:16
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Source\buzzer.c
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
    GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(BUZZER_GPIO_CLK);
    /* LED2 GPIO configuration */
    gpioConfig.pin = BUZZER_GPIO_PIN;
    gpioConfig.mode = GPIO_MODE_OUT;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(BUZZER_GPIO_PORT, &gpioConfig);

    BUZZER(0);
}

///**
// * @description: 蜂鸣器输出翻转
// * @return {*}
// */
//void buzzer_toggle(void)
//{
//    uint8_t value = (!GPIO_ReadOutputBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN));
//    GPIO_WriteBitValue(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, (GPIO_BSRET_T)value);
//}

///**
// * @description: 蜂鸣器快响3s
// * @return {*}
// */
//void buzzer_quick_3s(void)
//{
//    uint8_t work_count = 0;
//    while(work_count < 3)
//    {
//        BUZZER(0);
//        delay_nms(100);
//        BUZZER(1);
//        delay_nms(100);
//        work_count++;
//    }
//    BUZZER(0);
//}

///**
// * @description: 蜂鸣器按下快响100ms
// * @return {*}
// */
//void buzzer_quick_100ms(void)
//{
//    BUZZER(1);
//    delay_nms(100);
//    BUZZER(0);
//}
