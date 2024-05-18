/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-20 14:06:34
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-04-25 18:59:27
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Source\flash.c
 * @Description: flash模拟eeprom程序文件
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved. 
 */
#include "flash.h"

void flash_write_word(uint32_t addr, uint32_t data)
{
    /* Unlock Flash for Clear or Write*/
    FMC_Unlock();

    /* Write data in address*/
    FMC_ProgramWord(addr, data);

    /* Lock Flash*/
    FMC_Lock();

}

void flash_write_halfword(uint32_t addr, uint16_t data)
{
    /* Unlock Flash for Clear or Write*/
    FMC_Unlock();

    /* Write data in address*/
    FMC_ProgramHalfWord(addr, data);

    /* Lock Flash*/
    FMC_Lock();

}

/**
 * @description: 读取flash中指定字节的数据
 * @param {uint32_t} faddr
 * @return {*}
 */
uint32_t flash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @description: 读取flash中指定半字节的数据
 * @param {uint32_t} faddr
 * @return {*}
 */
uint16_t flash_read_halfword(uint32_t faddr)
{
    return *(volatile uint16_t *)faddr;
}
