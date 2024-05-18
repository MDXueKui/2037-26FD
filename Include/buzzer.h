/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:03:27
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-04-28 09:01:24
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Include\buzzer.h
 * @Description: 蜂鸣器驱动头文件
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved. 
 */
#ifndef __BUZZER_H
#define __BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f0xx_gpio.h"
#include "apm32f0xx_rcm.h"

#define BUZZER_GPIO_PIN                    GPIO_PIN_3
#define BUZZER_GPIO_PORT                   GPIOA
#define BUZZER_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

/******************************************************************************************/
/* 蜂鸣器开关定义 */
#define BUZZER(x)   do{ x ? \
                      GPIO_WriteBitValue(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* BUZZER */


extern uint32_t buzzer_time_count;

void buzzer_init(void);
//void buzzer_toggle(void);
//void buzzer_quick_3s(void);
//void buzzer_quick_100ms(void);

#ifdef __cplusplus
}
#endif

#endif
