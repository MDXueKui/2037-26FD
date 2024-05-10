/*
 * @Author: pengdong 2019262928@qq.com
 * @Date: 2024-05-08 17:16:30
 * @LastEditors: pengdong 2019262928@qq.com
 * @LastEditTime: 2024-05-10 10:15:01
 * @FilePath: \ediee:\pengdong\APM32F0xx_SDK_v1.7\Examples\RTOS\FreeRTOS\Source\uart_debug.c
 * @Description:
 * 版权声明 保留文件所有权利 2024
 * Copyright (c) 2024 by ${东莞市禹顺智能科技有限公司}, All Rights Reserved.
 */
#include "uart_debug.h"

void uart_debug_init(void)
{
    GPIO_Config_T gpioConfig;
    USART_Config_T usartConfigStruct;

    /* Enable GPIO clock */
    RCM_EnableAHBPeriphClock(DEBUG_USART_TX_GPIO_CLK);

    /* Enable COM1 clock*/
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB2PeriphClock(DEBUG_USART_TX_COM_CLK);

    /* Connect PXx to USARTx_Tx */
    GPIO_ConfigPinAF(DEBUG_USART_TX_PORT, DEBUG_USART_TX_SOURCEPIN, DEBUG_USART_TX_AF);
    GPIO_ConfigPinAF(DEBUG_USART_RX_PORT, DEBUG_USART_RX_SOURCEPIN, DEBUG_USART_RX_AF);

    /* Configure USART Tx as alternate function push-pull */
    gpioConfig.mode = GPIO_MODE_AF;
    gpioConfig.pin = DEBUG_USART_TX_PIN;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.pupd = GPIO_PUPD_PU;
    GPIO_Config(DEBUG_USART_TX_PORT, &gpioConfig);

    /* Configure USART Rx as input floating */
    gpioConfig.pin  = DEBUG_USART_RX_PIN;
    GPIO_Config(DEBUG_USART_RX_PORT, &gpioConfig);

    /* MINI_USARTs configured as follow: */
    /* BaudRate = 115200 baud */
    usartConfigStruct.baudRate = 115200;
    /* Receive and transmit enabled */
    usartConfigStruct.mode     = USART_MODE_TX_RX;
    /* Hardware flow control disabled (RTS and CTS signals) */
    usartConfigStruct.hardwareFlowCtrl = USART_FLOW_CTRL_NONE;
    /* No parity */
    usartConfigStruct.parity   = USART_PARITY_NONE;
    /* One Stop Bit */
    usartConfigStruct.stopBits =  USART_STOP_BIT_1;
    /* Word Length = 8 Bits */
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;
    /* USART_Config */
    USART_Config(DEBUG_USART, &usartConfigStruct);

    /* Enable USART_Interrupt_RXBNEIE */
    USART_EnableInterrupt(DEBUG_USART, USART_INT_RXBNEIE);

    NVIC_EnableIRQRequest(DEBUG_USART_IRQn, 2);

    /* Enable USART */
    USART_Enable(DEBUG_USART);
}


#if USART_EN /*如果使能了接收*/

/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t g_usart_rx_buf[USART_REC_LEN];

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
 *  bit13~0，    接收到的有效字节数目
*/
uint16_t g_usart_rx_sta = 0;

uint8_t g_rx_buffer[RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */

/*!
 * @brief        serial port tramsimt data
 *
 * @param        pointer to date that need to be sent
 *
 * @retval       None
 *
 * @note
 */
void USART_Write(USART_T* usart, uint8_t* dat)
{
    /* Enable USART_INT_TXCIE */
    USART_EnableInterrupt(usart, USART_INT_TXCIE);

    while (*dat)
    {
        while (USART_ReadStatusFlag(usart, USART_FLAG_TXBE) == RESET);

        USART_TxData(usart, *dat++);
    }

    USART_DisableInterrupt(usart, USART_INT_TXCIE);
}

/**
 * @brief       串口数据接收回调函数
                数据处理在这里进行
 * @param       huart:串口句柄
 * @retval      无
 */
void  USART_Receive_Isr(void)
{
    uint8_t g_rx_buffer;

    if (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_RXBNE) == SET)
    {
        g_rx_buffer = (uint8_t)USART_RxData(DEBUG_USART);

        if ((g_usart_rx_sta & 0x8000) == 0)             /* 接收未完成 */
        {
            if (g_usart_rx_sta & 0x4000)                /* 接收到了0x0d（即回车键） */
            {
                if (g_rx_buffer != 0x0a)             /* 接收到的不是0x0a（即不是换行键） */
                {
                    g_usart_rx_sta = 0;                 /* 接收错误,重新开始 */
                }
                else                                    /* 接收到的是0x0a（即换行键） */
                {
                    g_usart_rx_sta |= 0x8000;           /* 接收完成了 */
                }
            }
            else                                        /* 还没收到0X0d（即回车键） */
            {
                if (g_rx_buffer == 0x0d)
                {
                    g_usart_rx_sta |= 0x4000;
                }
                else
                {
                    g_usart_rx_buf[g_usart_rx_sta & 0X3FFF] = g_rx_buffer;
                    g_usart_rx_sta++;

                    if (g_usart_rx_sta > (USART_REC_LEN - 1))
                    {
                        g_usart_rx_sta = 0;             /* 接收数据错误,重新开始接收 */
                    }
                }
            }
        }
    }
}

#if defined (__CC_ARM) || defined (__ICCARM__) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @param       *f:  pointer to a FILE that can recording all information
*              needed to control a stream
*
* @retval      The characters that need to be send.
*
* @note
*/
int fputc(int ch, FILE* f)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART, (uint8_t)ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART, USART_FLAG_TXBE) == RESET);

    return (ch);
}

#elif defined (__GNUC__)

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       ch:  The characters that need to be send.
*
* @retval      The characters that need to be send.
*
* @note
*/
int __io_putchar(int ch)
{
    /* send a byte of data to the serial port */
    USART_TxData(DEBUG_USART_TX, ch);

    /* wait for the data to be send  */
    while (USART_ReadStatusFlag(DEBUG_USART_TX, USART_FLAG_TXBE) == RESET);

    return ch;
}

/*!
* @brief       Redirect C Library function printf to serial port.
*              After Redirection, you can use printf function.
*
* @param       file:  Meaningless in this function.
*
* @param       *ptr:  Buffer pointer for data to be sent.
*
* @param       len:  Length of data to be sent.
*
* @retval      The characters that need to be send.
*
* @note
*/
int _write(int file, char* ptr, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        __io_putchar(*ptr++);
    }

    return len;
}

#else
#warning Not supported compiler type
#endif

/*!
 * @brief        This function handles USART1 RX interrupt Handler
 *
 * @param        None
 *
 * @retval       None
 *
 * @note
 */
void USART1_IRQHandler(void)
{
    USART_Receive_Isr();
}

#endif
