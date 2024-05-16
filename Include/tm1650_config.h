#ifndef TM1650_CONFIG_H_
#define TM1650_CONFIG_H_
 
#include "tm1650.h"
 
//==========【配置IIC驱动引脚】========

#define TM1650_IIC_SCL_PIN       GPIO_PIN_13
#define TM1650_IIC_SCL_PORT      GPIOB
#define TM1650_IIC_SCL_CLK       RCM_AHB_PERIPH_GPIOB

#define TM1650_IIC_SDA_PIN       GPIO_PIN_12
#define TM1650_IIC_SDA_PORT      GPIOB
#define TM1650_IIC_SDA_CLK       RCM_AHB_PERIPH_GPIOB
 
//=====================================
 
 
//========【配置IIC总线的信号读写和时序】=======
//主机拉高SCL
#define TM1650_IIC_SCL_HIGH     GPIO_SetBit(TM1650_IIC_SCL_PORT,TM1650_IIC_SCL_PIN)
 
//主机拉低SCL
#define TM1650_IIC_SCL_LOW      GPIO_ClearBit(TM1650_IIC_SCL_PORT,TM1650_IIC_SCL_PIN)
 
 
//主机拉高SDA
#define TM1650_IIC_SDA_HIGH     GPIO_SetBit(TM1650_IIC_SDA_PORT,TM1650_IIC_SDA_PIN)
 
//主机拉低SDA
#define TM1650_IIC_SDA_LOW      GPIO_ClearBit(TM1650_IIC_SDA_PORT,TM1650_IIC_SDA_PIN)
 
//参数b为0时主机拉低SDA，非0则拉高SDA
#define TM1650_IIC_SDA_WR(b)    do{                                       \
                               if(b) GPIO_SetBit(TM1650_IIC_SDA_PORT,TM1650_IIC_SDA_PIN);   \
                               else  GPIO_ClearBit(TM1650_IIC_SDA_PORT,TM1650_IIC_SDA_PIN); \
                              }while(0)
 
 
//主机读取SDA线电平状态，返回值为0为低电平，非0则为高电平
#define TM1650_IIC_SDA_RD()    GPIO_ReadInputBit(TM1650_IIC_SDA_PORT,TM1650_IIC_SDA_PIN) 
 
//软件延时2us
#define TM1650_IIC_DELAY_2US   do{for(int ii_=0;ii_<50;ii_++);}while(0)
 
//软件延时4us
#define TM1650_IIC_DELAY_4US   do{for(int ii_=0;ii_<90;ii_++);}while(0)
//================================
 
 
#endif //TM1650_CONFIG_H_
