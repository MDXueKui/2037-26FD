/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-04-13 13:48:30
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-06 09:13:01
 * @FilePath: \MDKe:\pengdong\APM32F0xx_SDK_v1.7\Examples\GPIO\GPIO_Toggle\Source\iwdg.c
 * @Description: 看门狗程序，两秒复位
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "iwdg.h"

void IWDTInit(void)
{

    /* clear IWDTRST Flag*/
    if (RCM_ReadStatusFlag(RCM_FLAG_IWDTRST) != RESET)
    {
        RCM_ClearStatusFlag();
    }
    /* set IWDT Write Access */
    IWDT_EnableWriteAccess();

    /* set IWDT Divider*/
    IWDT_ConfigDivider(IWDT_DIV_64);

    /* 设置定时时间为4s 4s内没喂狗就重启 */
    IWDT_ConfigReload(IWDT_time * 40000 / 64);

    /* Refresh*/
    IWDT_Refresh();

    /* Enable IWDT*/
    IWDT_Enable();
}

/**
 * @description: IWDT_Refresh()的封装，喂狗函数
 * @return {*}
 */
void IWDT_Feed(void)
{
    IWDT_Refresh();
}
