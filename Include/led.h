/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-06 10:47:37
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-04-24 16:58:07
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Include\led.h
 * @Description: 
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved. 
 */
#ifndef __LED_H
#define __LED_H

#include "apm32f0xx_gpio.h"
#include "apm32f0xx_rcm.h"

#define LEDTEST_GPIO_PIN                 GPIO_PIN_0
#define LEDTEST_GPIO_PORT                GPIOB
#define LEDTEST_GPIO_CLK                 RCM_AHB_PERIPH_GPIOB

#define LED1_GPIO_PIN                    GPIO_PIN_13
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCM_AHB_PERIPH_GPIOC

#define LED2_GPIO_PIN                    GPIO_PIN_14
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCM_AHB_PERIPH_GPIOC

#define LED3_GPIO_PIN                    GPIO_PIN_0
#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

#define LED4_GPIO_PIN                    GPIO_PIN_5
#define LED4_GPIO_PORT                   GPIOA
#define LED4_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

#define LED8_GPIO_PIN                    GPIO_PIN_11
#define LED8_GPIO_PORT                   GPIOB
#define LED8_GPIO_CLK                    RCM_AHB_PERIPH_GPIOB

/******************************************************************************************/
/* LED1开关定义 */
#define LED1(x)   do{ x ? \
                      GPIO_WriteBitValue(LED1_GPIO_PORT, LED1_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED1_GPIO_PORT, LED1_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED2 */

/******************************************************************************************/
/* LED2开关定义 */
#define LED2(x)   do{ x ? \
                      GPIO_WriteBitValue(LED2_GPIO_PORT, LED2_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED2_GPIO_PORT, LED2_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED2 */
/******************************************************************************************/
/* LED3开关定义 */
#define LED3(x)   do{ x ? \
                      GPIO_WriteBitValue(LED3_GPIO_PORT, LED3_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED3_GPIO_PORT, LED3_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED3 */
/* LED4开关定义 */
#define LED4(x)   do{ x ? \
                      GPIO_WriteBitValue(LED4_GPIO_PORT, LED4_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED4_GPIO_PORT, LED4_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED4 */
/******************************************************************************************/
/* LED8开关定义 */
#define LED8(x)   do{ x ? \
                      GPIO_WriteBitValue(LED8_GPIO_PORT, LED8_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED8_GPIO_PORT, LED8_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED8 */
/* LEDTEST开关定义 */
#define LEDTEST(x)   do{ x ? \
                      GPIO_WriteBitValue(LEDTEST_GPIO_PORT, LEDTEST_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LEDTEST_GPIO_PORT, LEDTEST_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LEDTEST */

/* LED取反定义 */
#define LEDTEST_TOGGLE()   do{ GPIO_TogglePin(LEDTEST_GPIO_PORT, LEDTEST_GPIO_PIN); }while(0)       /* 反转LEDTEST */

void led_init(void);
void GPIO_TogglePin(GPIO_T* port, uint32_t pin);

#endif
