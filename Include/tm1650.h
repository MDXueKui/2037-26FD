/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-09 13:47:50
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-22 11:38:23
 * @FilePath: \ediee:\pengdong\APM32F0xx_SDK_v1.7\Examples\RTOS\FreeRTOS\Include\tm1650.h
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
/* 写flash事件标志位 */
#define flashw_eventbit3        			(1 << 3)
#define set_flashw_eventbit3(x) 			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, flashw_eventbit3)) : \
												(xEventGroupClearBits(EventGroupHandler, flashw_eventbit3)); \
											}while(0)

#define get_flashw_eventbit3    			(xEventGroupGetBits(EventGroupHandler) & flashw_eventbit3)
/* 开启事件标志位 */
#define on_eventbit4            			(1 << 4)
#define set_on_eventbit4(x)     			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, on_eventbit4)) : \
												(xEventGroupClearBits(EventGroupHandler, on_eventbit4)); \
											}while(0)

#define get_on_eventbit4        			(xEventGroupGetBits(EventGroupHandler) & on_eventbit4)
/* 关闭事件标志位 */
#define off_eventbit5           			(1 << 5)
#define set_off_eventbit5(x)    			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, off_eventbit5)) : \
												(xEventGroupClearBits(EventGroupHandler, off_eventbit5)); \
											}while(0)

#define get_off_eventbit5       		   	(xEventGroupGetBits(EventGroupHandler) & off_eventbit5)
/* 闪烁显示事件标志位 */
#define scintillation_eventbit6            	(1 << 6)
#define set_scintillation_eventbit6(x)    	 \
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, scintillation_eventbit6)) : \
												(xEventGroupClearBits(EventGroupHandler, scintillation_eventbit6)); \
											}while(0)

#define get_scintillation_eventbit6      	(xEventGroupGetBits(EventGroupHandler) & scintillation_eventbit6)
/* 消防事件标志位 */
#define fire_eventbit7            			(1 << 7)
#define set_fire_eventbit7(x)    			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, fire_eventbit7)) : \
												(xEventGroupClearBits(EventGroupHandler, fire_eventbit7)); \
											}while(0)

#define get_fire_eventbit7      			(xEventGroupGetBits(EventGroupHandler) & fire_eventbit7)
/* 老化事件标志位 */
#define aging_eventbit8            			(1 << 8)
#define set_aging_eventbit8(x)    			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, aging_eventbit8)) : \
												(xEventGroupClearBits(EventGroupHandler, aging_eventbit8)); \
											}while(0)

#define get_aging_eventbit8      			(xEventGroupGetBits(EventGroupHandler) & aging_eventbit8)
/* 上电时为6000模式事件标志位 */
#define loaded_eventbit9            		(1 << 9)
#define set_loaded_eventbit9(x)    			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, loaded_eventbit9)) : \
												(xEventGroupClearBits(EventGroupHandler, loaded_eventbit9)); \
											}while(0)

#define get_loaded_eventbit9      			(xEventGroupGetBits(EventGroupHandler) & loaded_eventbit9)
/* 级联输入R事件标志位 */
#define cascaded24vR_input_eventbit10       (1 << 10)
#define set_cascaded24vR_input_eventbit10(x)\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, cascaded24vR_input_eventbit10)) : \
												(xEventGroupClearBits(EventGroupHandler, cascaded24vR_input_eventbit10)); \
											}while(0)

#define get_cascaded24vR_input_eventbit10   (xEventGroupGetBits(EventGroupHandler) & cascaded24vR_input_eventbit10)
/* 级联输入L事件标志位 */
#define cascaded24vL_input_eventbit11       (1 << 11)
#define set_cascaded24vL_input_eventbit11(x)\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, cascaded24vL_input_eventbit11)) : \
												(xEventGroupClearBits(EventGroupHandler, cascaded24vL_input_eventbit11)); \
											}while(0)

#define get_cascaded24vL_input_eventbit11   (xEventGroupGetBits(EventGroupHandler) & cascaded24vL_input_eventbit11)
/* 锁定事件标志位 */
#define lock_eventbit12                     (1 << 12)
#define set_lock_eventbit12(x)    			\
											do{ x ? \
												(xEventGroupSetBits(EventGroupHandler, lock_eventbit12)) : \
												(xEventGroupClearBits(EventGroupHandler, lock_eventbit12)); \
											}while(0)

#define get_lock_eventbit12                 (xEventGroupGetBits(EventGroupHandler) & lock_eventbit12)



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

extern uint16_t tube_display_buff[4];
extern uint32_t scintillation_time;
extern uint8_t scintillation_time_count;
extern uint16_t up_time_buff;
extern uint16_t down_time_buff;
extern uint16_t fire_time_buff;
extern uint16_t aging_time_buff;
extern uint16_t ManualAuto_flag;
extern uint32_t aging_time_count;
extern uint8_t aging_mode;
extern uint16_t temp_time_buff;
extern uint32_t on_off_time_count; /* 电机上行下行时间计数变量 */

void TM1650_init(void);
void TM1650_cfg_display(uint8_t param);
void TM1650_clear(void);
void TM1650_print(uint8_t dig,uint8_t seg_data);
void Dispaly_Task (void* parameter);

#endif //TM1650_H_
