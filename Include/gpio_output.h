/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:07:30
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-20 19:48:32
 * @FilePath: \FreeRTOS\Include\gpio_output.h
 * @Description: gpio输出控制头文件
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#ifndef __GPIO_OUTPUT_H
#define __GPIO_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f0xx_gpio.h"
#include "apm32f0xx_rcm.h"

#define UPOUT1_GPIO_PIN                    GPIO_PIN_4
#define UPOUT1_GPIO_PORT                   GPIOA
#define UPOUT1_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

#define DNOUT1_GPIO_PIN                    GPIO_PIN_5
#define DNOUT1_GPIO_PORT                   GPIOA
#define DNOUT1_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

//#define UPOUT2_GPIO_PIN                    GPIO_PIN_5
//#define UPOUT2_GPIO_PORT                   GPIOA
//#define UPOUT2_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

//#define DNOUT2_GPIO_PIN                    GPIO_PIN_6
//#define DNOUT2_GPIO_PORT                   GPIOA
//#define DNOUT2_GPIO_CLK                    RCM_AHB_PERIPH_GPIOA

// #define cascaded_outputA_GPIO_PIN          GPIO_PIN_11
// #define cascaded_outputA_GPIO_PORT         GPIOA
// #define cascaded_outputA_GPIO_CLK          RCM_AHB_PERIPH_GPIOA

// #define cascaded_outputB_GPIO_PIN          GPIO_PIN_12
// #define cascaded_outputB_GPIO_PORT         GPIOA
// #define cascaded_outputB_GPIO_CLK          RCM_AHB_PERIPH_GPIOA

#define firefeedback_output_GPIO_PIN       GPIO_PIN_8
#define firefeedback_output_PORT           GPIOA
#define firefeedback_output_CLK            RCM_AHB_PERIPH_GPIOA

// #define controlfeedback_GPIO_PIN           GPIO_PIN_11
// #define controlfeedback_GPIO_PORT          GPIOB
// #define controlfeedback_GPIO_CLK           RCM_AHB_PERIPH_GPIOB

#define LED1_GPIO_PIN                      GPIO_PIN_0
#define LED1_GPIO_PORT                     GPIOA
#define LED1_GPIO_CLK                      RCM_AHB_PERIPH_GPIOA

#define LED2_GPIO_PIN                      GPIO_PIN_15
#define LED2_GPIO_PORT                     GPIOC
#define LED2_GPIO_CLK                      RCM_AHB_PERIPH_GPIOC

#define LED3_GPIO_PIN                      GPIO_PIN_14
#define LED3_GPIO_PORT                     GPIOC
#define LED3_GPIO_CLK                      RCM_AHB_PERIPH_GPIOC
/* 自动模式下LED4亮，手动模式下灭 */
#define LED4_GPIO_PIN                      GPIO_PIN_1
#define LED4_GPIO_PORT                     GPIOA
#define LED4_GPIO_CLK                      RCM_AHB_PERIPH_GPIOA
/* UPOUT1输出定义 */
#define UPOUT1(x)   do{ x ? \
                      GPIO_WriteBitValue(UPOUT1_GPIO_PORT, UPOUT1_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(UPOUT1_GPIO_PORT, UPOUT1_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* UPOUT1 */
/******************************************************************************************/

/* DNOUT1输出定义 */
#define DNOUT1(x)   do{ x ? \
                      GPIO_WriteBitValue(DNOUT1_GPIO_PORT, DNOUT1_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(DNOUT1_GPIO_PORT, DNOUT1_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* DNOUT1 */
/******************************************************************************************/

///* UPOUT2输出定义 */
//#define UPOUT2(x)   do{ x ? \
//                      GPIO_WriteBitValue(UPOUT2_GPIO_PORT, UPOUT2_GPIO_PIN, Bit_SET) : \
//                      GPIO_WriteBitValue(UPOUT2_GPIO_PORT, UPOUT2_GPIO_PIN, Bit_RESET); \
//                  }while(0)      /* UPOUT2 */
///******************************************************************************************/

///* DNOUT2输出定义 */
//#define DNOUT2(x)   do{ x ? \
//                      GPIO_WriteBitValue(DNOUT2_GPIO_PORT, DNOUT2_GPIO_PIN, Bit_SET) : \
//                      GPIO_WriteBitValue(DNOUT2_GPIO_PORT, DNOUT2_GPIO_PIN, Bit_RESET); \
//                  }while(0)      /* DNOUT2 */

/******************************************************************************************/
/* 级联输出A输出定义 */
// #define cascaded_outputA(x)   do{ x ? \
//                       GPIO_WriteBitValue(cascaded_outputA_GPIO_PORT, cascaded_outputA_GPIO_PIN, Bit_SET) : \
//                       GPIO_WriteBitValue(cascaded_outputA_GPIO_PORT, cascaded_outputA_GPIO_PIN, Bit_RESET); \
//                   }while(0)      /* cascaded_outputA */

// /******************************************************************************************/
// /* 级联输出B输出定义 */
// #define cascaded_outputB(x)   do{ x ? \
//                       GPIO_WriteBitValue(cascaded_outputB_GPIO_PORT, cascaded_outputB_GPIO_PIN, Bit_SET) : \
//                       GPIO_WriteBitValue(cascaded_outputB_GPIO_PORT, cascaded_outputB_GPIO_PIN, Bit_RESET); \
//                   }while(0)      /* cascaded_outputB */

/******************************************************************************************/
/* 消防反馈输出定义 */
#define firefeedback_output(x)   do{ x ? \
                      GPIO_WriteBitValue(firefeedback_output_PORT, firefeedback_output_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(firefeedback_output_PORT, firefeedback_output_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* firefeedback_output */
/******************************************************************************************/
/* 受控反馈输出定义 */
// #define controlfeedback_output(x)   do{ x ? \
//                       GPIO_WriteBitValue(controlfeedback_GPIO_PORT, controlfeedback_GPIO_PIN, Bit_SET) : \
//                       GPIO_WriteBitValue(controlfeedback_GPIO_PORT, controlfeedback_GPIO_PIN, Bit_RESET); \
//                   }while(0)      /* controlfeedback_output */
/******************************************************************************************/
/* 受控反馈输出LED定义 */
#define LED1(x)   do{ x ? \
                      GPIO_WriteBitValue(LED1_GPIO_PORT, LED1_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED1_GPIO_PORT, LED1_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED1 */
/******************************************************************************************/
/* 受控反馈输出LED定义 */
#define LED2(x)   do{ x ? \
                      GPIO_WriteBitValue(LED2_GPIO_PORT, LED2_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED2_GPIO_PORT, LED2_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED1 */
/******************************************************************************************/
/* 消防来时LED定义 */
#define LED2(x)   do{ x ? \
                      GPIO_WriteBitValue(LED2_GPIO_PORT, LED2_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED2_GPIO_PORT, LED2_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED2 */
/******************************************************************************************/
/* 电机运行LED定义 */
#define LED3(x)   do{ x ? \
                      GPIO_WriteBitValue(LED3_GPIO_PORT, LED3_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED3_GPIO_PORT, LED3_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED3 */
/******************************************************************************************/
/* 手自动切换LED定义 */
#define LED4(x)   do{ x ? \
                      GPIO_WriteBitValue(LED4_GPIO_PORT, LED4_GPIO_PIN, Bit_SET) : \
                      GPIO_WriteBitValue(LED4_GPIO_PORT, LED4_GPIO_PIN, Bit_RESET); \
                  }while(0)      /* LED4 */
/******************************************************************************************/

void gpio_output_init(void);
void motor_up(void);
void motor_down(void);
void motor_stop(void);
void stop_motor(void);
void motor_on_all(void);
void motro_off_all(void);
void fire_proc(void);

#ifdef __cplusplus
}
#endif

#endif
