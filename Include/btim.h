/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-05-13 09:25:06
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-13 10:38:17
 * @FilePath: \ediee:\pengdong\APM32F0xx_SDK_v1.7\Examples\RTOS\FreeRTOS\Include\btim.h
 * @Description:
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#ifndef __BTIM_H
#define __BTIM_H

#include "apm32f0xx.h"
#include "apm32f0xx_tmr.h"
#include "apm32f0xx_misc.h"
#include "apm32f0xx_rcm.h"

void btim_tim6_int_init(uint16_t arr, uint16_t psc);
void ConfigureTimeForRunTimeStats(void);
void TMR6_Isr(void);

#endif

















