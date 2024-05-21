/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:07:30
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-21 10:39:44
 * @FilePath: \FreeRTOS\Include\rf_receive.h
 * @Description: 接收程序头文件
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#ifndef __RF_RECEIVE_H
#define __RF_RECEIVE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "apm32f0xx_gpio.h"
#include "apm32f0xx_tmr.h"
#include "apm32f0xx_rcm.h"
#include "apm32f0xx_misc.h"
#include "uart_debug.h"
#include "buzzer.h"
#include "flash.h"
#include "tm1650.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define learn_eventbit_1        (1 << 1)
#define set_learn_eventbit_1(x) \
                                do{ x ? \
                                    (xEventGroupSetBits(EventGroupHandler, learn_eventbit_1)) : \
                                    (xEventGroupClearBits(EventGroupHandler, learn_eventbit_1)); \
                                }while(0)

#define get_learn_eventbit_1    (xEventGroupGetBits(EventGroupHandler) & learn_eventbit_1)

#define id_data_max 10

#define data_gpio_pin           GPIO_PIN_5
#define data_gpio_source_pin    GPIO_PIN_SOURCE_5
#define data_AF_pin             GPIO_AF_PIN1
#define data_gpio_port          GPIOB
#define data_input_channel      TMR_CHANNEL_2
#define data_gpio_clock         RCM_AHB_PERIPH_GPIOB
#define data_tim_clock          RCM_APB1_PERIPH_TMR3

extern uint8_t g_rec_time;
extern uint8_t g_rec_timesave;
extern uint8_t g_out_fg1;
extern uint8_t g_out_data1;

void rf_receive_Init(void); /* 接收初始化函数 */
void Rf_Rec_Pro_Task(void *parameter);

#ifdef __cplusplus
}
#endif

#endif
