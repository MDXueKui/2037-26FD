/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-09 13:46:58
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-16 15:56:58
 * @FilePath: \ediee:\pengdong\APM32F0xx_SDK_v1.7\Examples\RTOS\FreeRTOS\Source\tm1650.c
 * @Description:
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "tm1650.h"
#include "tm1650_config.h"
<<<<<<< HEAD
#include "FreeRTOS.h"
#include "event_groups.h"
=======
>>>>>>> e6b1db919a3404ff6bbfd9ab2115edcd36ea1229

/* 往一个指定的数码管位写入指定的显示数据 */
/* 共阴数码管段码表 */
/* 此处不知道是不是数码管接错了  现在的变化示例：1   0000 0110    6、7位  2、3位分别反过来后交换位置 */
/* 这个地方如果数码管接对了直接用通用的数就行 */
const uint8_t TUBE_TABLE_0[17] = {0xf3,0x82,0x5b,0xcb,0xaa,0xe9,0xf9,0x83,0xfb,0xeb,0xbb,0xf8,0x71,0xda,0x79,0x39, 0x00};

uint16_t tube_display_buff[4] = {0, 0, 0, 0};
uint16_t up_time_buff = 30;  /* 电机上行时间 */
uint16_t down_time_buff = 30;  /* 电机下行时间 */
uint16_t fire_time_buff = 10;  /* 消防上行时间 */
uint16_t aging_time_buff = 10;	/* 老化时间 */
uint16_t ManualAuto_flag = 0; /* 手动自动模式标志位 */
uint16_t temp_time_buff = 1000;  /* 为了老化时间的显示逻辑正常，所以设置为1000 */
uint8_t aging_flag = 0;
uint32_t aging_time_count = 0;
uint8_t aging_mode = 0;
uint8_t tube_change_flag = 0;  /* 数码管刷新显示标志 */
uint8_t scintillation_flag = 0;  /* 设置时间是数码管闪烁显示的标志 */
uint32_t scintillation_time = 0; /* 放在systick中用于控制闪烁和判断设置时间的变量 */
uint8_t scintillation_time_count = 0;
#define tm1650_max 6
uint16_t *point[tm1650_max] = {&tube_display_buff[0], &up_time_buff, &down_time_buff, &fire_time_buff, &aging_time_buff, &ManualAuto_flag};

/**
 * @description: 将模式值，电机上行时间，下行时间，消防时间，老化时间
 *								 从对应的flash地址中取出
 * @return {*}
 */
//void tm1650_data_init(void)
//{
//	uint32_t tm1650_data_addr = 0x0800f800;
//	uint8_t i = 0;
//	/* Unlock Flash for Clear or Write*/
//    FMC_Unlock();

//	for(i = 0; i < tm1650_max; i++)
//	{
//		*point[i] = flash_read_halfword(tm1650_data_addr + i * 2);
//		printf("flash_addr = 0x%08x,flash_id_data = %d\n", tm1650_data_addr + i * 2, *point[i]);
//	}

//	/* Lock Flash*/
//    FMC_Lock();
//}

/**
 * @description: 将模式值，电机上行时间，下行时间，消防时间，老化时间
 *								 写入对应的flash地址
 * @return {*}
 */
//void tm1650_data_write(void)
//{
//	uint32_t tm1650_data_addr = 0x0800f800;
//	FMC_Unlock();
//	FMC_ErasePage(tm1650_data_addr);
//	FMC_Lock();
//	uint8_t i = 0;
//	for(i = 0; i < tm1650_max; i++)
//	{
//		if((on_flag == 1 || off_flag == 1) && i == 0)
//		{
//			flash_write_halfword(tm1650_data_addr + i * 2, 2);
//		}
//		else
//		{
//			flash_write_halfword(tm1650_data_addr + i * 2, *point[i]);
//		}
//		printf("flash_addr = 0x%08x,flash_id_data = %d\n", tm1650_data_addr + i * 2, flash_read_halfword(tm1650_data_addr + i * 2));
//	}

//}

/**
 * @description: 产生IIC总线起始信号
 * @return {*}
 */
static void TM1650_IIC_start(void)
{
	TM1650_IIC_SCL_HIGH;     //SCL=1
	TM1650_IIC_SDA_HIGH;    //SDA=1
	TM1650_IIC_DELAY_4US;
	TM1650_IIC_SDA_LOW;     //SDA=0
	TM1650_IIC_DELAY_4US;
	TM1650_IIC_SCL_LOW;      //SCL=0
}

//产生IIC总线结束信号
static void TM1650_IIC_stop(void)
{
	TM1650_IIC_SCL_LOW;      //SCL=0
	TM1650_IIC_SDA_LOW;      //SDA=0
	TM1650_IIC_DELAY_4US;
	TM1650_IIC_SCL_HIGH;     //SCL=1
	TM1650_IIC_DELAY_4US;
	TM1650_IIC_SDA_HIGH;    //SDA=1
}

//通过IIC总线发送一个字节
static void TM1650_IIC_write_byte(uint8_t dat)
{
	uint8_t i;

	TM1650_IIC_SCL_LOW;
	for(i=0;i<8;i++)
	{
		TM1650_IIC_SDA_WR(dat&0x80);
		dat<<=1;

		TM1650_IIC_DELAY_2US;
		TM1650_IIC_SCL_HIGH;
		TM1650_IIC_DELAY_2US;
		TM1650_IIC_SCL_LOW;
		TM1650_IIC_DELAY_2US;
	}
}

//通过IIC总线接收从机响应的ACK信号
static uint8_t TM1650_IIC_wait_ack(void)
{
	uint8_t ack_signal = 0;

	TM1650_IIC_SDA_HIGH;    //SDA=1
	TM1650_IIC_DELAY_2US;
	TM1650_IIC_SCL_HIGH;
	TM1650_IIC_DELAY_2US;
	if(TM1650_IIC_SDA_RD()) ack_signal = 1;
	TM1650_IIC_SCL_LOW;
	TM1650_IIC_DELAY_2US;
	return ack_signal;
}



/**
 * @description: TM1650初始化
 * @return {*}
 */
void TM1650_init(void)
{
//	tm1650_data_init();
	GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(TM1650_IIC_SCL_CLK);
    /* TM1650_IIC_SCL GPIO configuration */
    gpioConfig.pin = TM1650_IIC_SCL_PIN;
    gpioConfig.mode = GPIO_MODE_OUT;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(TM1650_IIC_SCL_PORT, &gpioConfig);

    /* TM1650_IIC_SCL GPIO configuration */
    gpioConfig.pin = TM1650_IIC_SDA_PIN;
    GPIO_Config(TM1650_IIC_SDA_PORT, &gpioConfig);

    TM1650_IIC_SDA_HIGH;
    TM1650_IIC_SCL_HIGH;
	/* 打开显示且使亮度等级为1 */
	TM1650_cfg_display(TM1650_BRIGHT1);
    TM1650_clear();
}


//作用：设置显示参数
//备注：这个操作不影响显存中的数据
//用例：
//	设置亮度并打开显示:TM1650_cfg_display(TM1650_BRIGHTx)
//	关闭显示:TM1650_cfg_display(TM1650_DSP_OFF)
void TM1650_cfg_display(uint8_t param)
{
	TM1650_IIC_start();
	TM1650_IIC_write_byte(0x48);
	TM1650_IIC_wait_ack();
	TM1650_IIC_write_byte(param);
	TM1650_IIC_wait_ack();
	TM1650_IIC_stop();
}


//将显存数据全部刷为0，清空显示
void TM1650_clear(void)
{
	uint8_t dig;
	for(dig = TM1650_DIG1 ; dig<= TM1650_DIG4 ;dig++)
	{
		TM1650_print(dig,TUBE_TABLE_0[2]);
	}
}


//用例：
//	在DIG1位上显示数字3: TM1650_print(TM1650_DIG1,TUBE_TABLE_0[3]);
void TM1650_print(uint8_t dig,uint8_t seg_data)
{
	TM1650_IIC_start();
	TM1650_IIC_write_byte(dig*2+0x68);
	TM1650_IIC_wait_ack();
	TM1650_IIC_write_byte(seg_data);
	TM1650_IIC_wait_ack();
	TM1650_IIC_stop();
}

<<<<<<< HEAD
extern EventGroupHandle_t EventGroupHandler;

=======
>>>>>>> e6b1db919a3404ff6bbfd9ab2115edcd36ea1229
void Dispaly_Task(void* parameter)
{
	printf("Dispaly_Task start!\r\n");
	while(1)
	{
<<<<<<< HEAD
        xEventGroupWaitBits((EventGroupHandle_t )EventGroupHandler, /* 等待的事件标志组句柄 */
                    (EventBits_t        )((1 << 0)),      /* 等待的事件 */
                    (BaseType_t         )pdTRUE,            /* 函数退出时清零等待的事件 */
                    (BaseType_t         )pdTRUE,            /* 等待等待的事件中的所有事件 */
                    (TickType_t         )portMAX_DELAY);    /* 等待时间 */
		printf("Dispaly_Task start_test!\n");
        IWDT_Feed();
=======
//		printf("Dispaly_Task start_test!\n");
//        IWDT_Feed();
>>>>>>> e6b1db919a3404ff6bbfd9ab2115edcd36ea1229
		TM1650_cfg_display(TM1650_BRIGHT1);
		TM1650_print(TM1650_DIG1, TUBE_TABLE_0[tube_display_buff[0]]);
		TM1650_print(TM1650_DIG2, TUBE_TABLE_0[tube_display_buff[1]]);
		TM1650_print(TM1650_DIG3, TUBE_TABLE_0[tube_display_buff[2]]);
		TM1650_print(TM1650_DIG4, TUBE_TABLE_0[tube_display_buff[3]]);
		vTaskDelay(20);
	}
}

//void display_change(uint16_t buff)
//{
//    tube_display_buff[1] = buff/100;
//    tube_display_buff[2] = buff%100/10;
//    tube_display_buff[3] = buff%10;
//}

//void clear_all_flag(void)
//{
//	on_flag = 0;
//	off_flag = 0;
//	scintillation_flag = 0;
//}

//void mode_judge(void)
//{

//	if(ManualAuto_flag)
//	{
//		LED4(0);
//	}
//	else
//	{
//		LED4(1);
//	}

//   switch(tube_display_buff[0])
//	{
//		case 1:
//			display_change(0);
//			break;
//		case 2:
//			if(aging_flag == 1)
//			{
//				aging_flag = 0;
//				aging_mode = 0;
//                aging_time_count = 0;
//				motor_stop();
//				display_change(0);
//			}
//			else
//			{
//				display_change(0);
//			}
//			break;
//		case 3:
//			display_change(up_time_buff);
//			break;
//		case 4:
//			display_change(down_time_buff);
//			break;
//		case 5:
//			if(fire_flag)
//			{
//				display_change(temp_time_buff);
//			}
//			else
//			{
//				if(aging_flag == 1)
//				{
//					aging_flag = 0;
//					aging_mode = 0;
//					aging_time_count = 0;
//					motor_stop();
//					display_change(0);
//				}
//				motor_stop();
//				display_change(fire_time_buff);
//			}
//			break;
//		case 7:
//			if(on_flag)
//			{
//				display_change(temp_time_buff);
//			}
//			else
//			{
//				motor_stop();
//				display_change(0);
//			}
//			break;
//		case 8:
//			if(off_flag)
//			{
//				display_change(temp_time_buff);
//			}
//			else
//			{
//				aging_flag = 0;
//				aging_mode = 0;
//                aging_time_count = 0;
//                temp_time_buff = 1000;
//				motor_stop();
//				display_change(0);
//			}
//			break;
//		case 9:
//			if(scintillation_flag == 1)
//			{
//				aging_flag = 0;
//				temp_time_buff = 1000;
//				aging_time_count = 0;
//			}
//			else
//			{
//				aging_flag = 1;
//			}
//			if(aging_flag)
//			{
//				if(temp_time_buff == 1000)
//				{
//					temp_time_buff = aging_time_buff;
//				}
//				display_change(temp_time_buff);
//			}
//			else
//			{
//				display_change(aging_time_buff);
//			}
//			break;
//		default:
//			display_change(0);
//			break;
//	}
//}

//void tube_display(void)
//{
//	mode_judge();
//	TM1650_cfg_display(TM1650_BRIGHT1);
//	TM1650_print(TM1650_DIG1, TUBE_TABLE_0[tube_display_buff[0]]);
//	TM1650_print(TM1650_DIG2, TUBE_TABLE_0[tube_display_buff[1]]);
//	TM1650_print(TM1650_DIG3, TUBE_TABLE_0[tube_display_buff[2]]);
//	TM1650_print(TM1650_DIG4, TUBE_TABLE_0[tube_display_buff[3]]);
//}

///**
// * @description: 数码管闪烁显示模式，每0.5s闪烁一次
// * @return {*}
// */
//void tube_scintillation_display(void)
//{
//	mode_judge();
//    if(scintillation_time_count >= 5)
//    {
//        scintillation_time_count = 0;
//        scintillation_flag = 0;
//        scintillation_time = 0;
//        tube_change_flag = 1;
//    }
//	if(scintillation_time < 500)
//	{
//		TM1650_print(TM1650_DIG2, TUBE_TABLE_0[16]);
//		TM1650_print(TM1650_DIG3, TUBE_TABLE_0[16]);
//		TM1650_print(TM1650_DIG4, TUBE_TABLE_0[16]);
//	}
//	if(scintillation_time >= 500)
//	{
//		TM1650_print(TM1650_DIG2, TUBE_TABLE_0[tube_display_buff[1]]);
//		TM1650_print(TM1650_DIG3, TUBE_TABLE_0[tube_display_buff[2]]);
//		TM1650_print(TM1650_DIG4, TUBE_TABLE_0[tube_display_buff[3]]);
//	}
//}
