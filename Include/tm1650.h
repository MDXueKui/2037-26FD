/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-09 13:47:50
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-06 19:57:06
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Include\tm1650.h
 * @Description: 数码管驱动程序头文件
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#ifndef TM1650_H_
#define TM1650_H_
#include "stdint.h"
#include "apm32f0xx_rcm.h"
#include "apm32f0xx_gpio.h"
#include "uart_debug.h"
#include "input_scan.h"
#include "iwdg.h"

//显示参数
#define TM1650_BRIGHT1       0x11   /*一级亮度，打开LED显示*/
#define TM1650_BRIGHT2       0x21   /*二级亮度，打开LED显示*/
#define TM1650_BRIGHT3       0x31   /*三级亮度，打开LED显示*/
#define TM1650_BRIGHT4       0x41   /*四级亮度，打开LED显示*/
#define TM1650_BRIGHT5       0x51   /*五级亮度，打开LED显示*/
#define TM1650_BRIGHT6       0x61   /*六级亮度，打开LED显示*/
#define TM1650_BRIGHT7       0x71   /*七级亮度，打开LED显示*/
#define TM1650_BRIGHT8       0x01   /*八级亮度，打开LED显示*/
#define TM1650_DSP_OFF       0x00   /*关闭LED显示*/

//数码管位选
#define TM1650_DIG1     0
#define TM1650_DIG2     1
#define TM1650_DIG3     2
#define TM1650_DIG4     3

//extern const uint8_t TUBE_TABLE_0[17];
//extern uint8_t tube_change_flag;
extern uint16_t tube_display_buff[4];
//extern uint32_t scintillation_time;
//extern uint8_t scintillation_time_count;
//extern uint16_t up_time_buff;
//extern uint16_t down_time_buff;
//extern uint16_t fire_time_buff;
//extern uint16_t aging_time_buff;
//extern uint8_t aging_flag;
//extern uint32_t aging_time_count;
//extern uint8_t aging_mode;
//extern uint16_t temp_time_buff;
//extern uint8_t scintillation_flag;
//extern uint16_t ManualAuto_flag;

void TM1650_init(void);
void TM1650_cfg_display(uint8_t param);
void TM1650_clear(void);
void TM1650_print(uint8_t dig,uint8_t seg_data);
void Dispaly_Task (void* parameter);
//void tube_scintillation_display(void);
//void mode_judge(void);
//void tm1650_data_write(void);

#endif //TM1650_H_
