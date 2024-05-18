/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-06 10:03:46
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-14 18:53:40
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Source\Systick_delay.c
 * @Description: 使用systick定时器实现延时以及计时的函数
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "systick_delay.h"

uint32_t sysTick = 0;

/**
 * @brief 延时函数初始化，配置系统滴答定时器，1ms中断一次
 * @return {*}
 * @note
 */
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1);
    }
}

/*!
 * @brief       延时nus
 * @note        nus取值范围：0~2^32 / SystemCoreClock
 * @param       nus:要延时的us数
 * @retval      None
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = nus * (48);                 /* 需要的节拍数 */

    told = SysTick->VAL;                    /* 刚进入的计时器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计时器 */
            }
            else
            {
                tcnt += reload - tnow + told;  /* 这里要考虑定时器递减的值减到0后重载 */
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                      /* 时间要超过或等于要延迟的时间则退出 */
            }
        }
    }
}

/**
 * @brief ms级延时函数，使用us级延时函数实现
 * @param {uint32_t} nms
 * @return {*}
 * @note
 */
void delay_nms(uint32_t nms)
{
    delay_us((uint32_t)(1000 * nms));
}


/*!
 * @brief 定时器中断函数
 * @return {*}
 */
void SysTick_Handler(void)
{
    sysTick++;
    if(scintillation_flag)
    {
        scintillation_time++;
        if(scintillation_time > 1000)
        {
            scintillation_time_count++;
            scintillation_time = 0;
        }
    }
	else if(on_flag)
	{
		on_off_time_count++;
		if(on_off_time_count >= 1000)
		{
			temp_time_buff--;
			on_off_time_count = 0;
		}
		if(temp_time_buff > up_time_buff)
		{
            tube_display_buff[0] = 2;
            motor_stop();
            tube_change_flag = 1;
			on_flag = 0;
		}
	}
	else if(off_flag)
	{
		on_off_time_count++;
		if(on_off_time_count >= 1000)
		{
			temp_time_buff--;
			on_off_time_count = 0;
		}
		if(temp_time_buff > down_time_buff)
		{
            tube_display_buff[0] = 2;
            motor_stop();
            tube_change_flag = 1;
			off_flag = 0;
		}
	}
    else if(fire_flag && (tube_display_buff[0] != 6) && \
                    (fire_time_buff != 0))
	{
		on_off_time_count++;
		if(on_off_time_count >= 1000)
		{
			temp_time_buff--;
			on_off_time_count = 0;
		}
		if(temp_time_buff > fire_time_buff)
		{
            temp_time_buff = 0;
            motor_stop();
		}
	}
    else if(aging_flag)
    {
        aging_time_count++;
		if(aging_time_count >= 1000)
		{
			temp_time_buff--;
			aging_time_count = 0;
		}
		if(temp_time_buff > aging_time_buff)
		{
            switch(aging_mode)
            {
                case 0:
                    motor_up();
                    aging_mode++;
                    break;
                case 1:
                    motor_stop();
                    aging_mode++;
                    break;
                case 2:
                    motor_down();
                    aging_mode++;
                    break;
                case 3:
                    motor_stop();
                    aging_mode = 0;
                    break;
                default:
                    break;
            }
            temp_time_buff = aging_time_buff;
            tube_change_flag = 1;
		}
    }
    if(learn_flag)
    {
        buzzer_time_count++;
        if(buzzer_time_count % 500 == 0)
        {
            buzzer_toggle();
        }
        if(buzzer_time_count >= 10000)
        {
            buzzer_time_count = 0;
            learn_flag = 0;
        }
    }
}

uint32_t get_systick_value(void)
{
    return sysTick;
}
