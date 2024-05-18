/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-11 17:07:30
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-04-25 17:13:55
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Include\rf_receive.h
 * @Description: 接收程序头文件
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved. 
 */
#ifndef __RF_RECEIVE_H
#define __RF_RECEIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "apm32f0xx_gpio.h"
#include "apm32f0xx_tmr.h"
#include "apm32f0xx_rcm.h"
#include "apm32f0xx_misc.h"
#include "uart_debug.h"
#include "buzzer.h"
#include "flash.h"

#define id_data_max 10

#define data_gpio_pin           GPIO_PIN_5
#define data_gpio_source_pin    GPIO_PIN_SOURCE_5
#define data_AF_pin             GPIO_AF_PIN1
#define data_gpio_port          GPIOB
#define data_input_channel      TMR_CHANNEL_2
#define data_gpio_clock         RCM_AHB_PERIPH_GPIOB
#define data_tim_clock          RCM_APB1_PERIPH_TMR3

/* apm32flash 预留做eeprom大小 */
#define flash_eeprom_base 0x0800FC00 /* 预留区域的起始地址 */
#define flash_eeprom_size 1024 /* 预留区域的大小 */

extern uint8_t g_rec_time;
extern uint8_t g_rec_timesave;
extern uint8_t g_rec_fg;
extern uint8_t g_out_fg1;
extern uint8_t g_out_data1;
extern uint8_t learn_flag;

void rf_receive_Init(void); /* 接收初始化函数 */
void rec_pro_ys(void); /* 接收处理函数 */
void rf_data_pro(void); /* 接收控制函数 */

#ifdef __cplusplus
}
#endif

#endif
