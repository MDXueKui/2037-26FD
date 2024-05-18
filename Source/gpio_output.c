/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:06:37
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-13 15:50:03
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Source\gpio_output.c
 * @Description:
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "gpio_output.h"

void gpio_output_init(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(UPOUT1_GPIO_CLK | cascaded_outputB_GPIO_CLK | LED1_GPIO_CLK);
    gpioConfig.pin = UPOUT1_GPIO_PIN;
    gpioConfig.mode = GPIO_MODE_OUT;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(UPOUT1_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = DNOUT1_GPIO_PIN;
    GPIO_Config(DNOUT1_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = UPOUT2_GPIO_PIN;
    GPIO_Config(UPOUT2_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = DNOUT2_GPIO_PIN;
    GPIO_Config(DNOUT2_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = cascaded_outputA_GPIO_PIN;
    GPIO_Config(cascaded_outputA_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = cascaded_outputB_GPIO_PIN;
    GPIO_Config(cascaded_outputB_GPIO_PORT, &gpioConfig);

    gpioConfig.pin = firefeedback_output_GPIO_PIN;
    GPIO_Config(firefeedback_output_PORT, &gpioConfig);

    gpioConfig.pin = controlfeedback_GPIO_PIN;
    GPIO_Config(controlfeedback_GPIO_PORT, &gpioConfig);

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
    UPOUT2(0);
    DNOUT2(0);
    cascaded_outputA(0);
    cascaded_outputB(0);
    firefeedback_output(0);
    controlfeedback_output(0);
}
