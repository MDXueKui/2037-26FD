#ifndef __UART_DEBUG_H
#define __UART_DEBUG_H

#include "apm32f0xx_gpio.h"
#include "apm32f0xx_usart.h"
#include "apm32f0xx_misc.h"
#include "apm32f0xx_rcm.h"
#include "stdio.h"

#define DEBUG_USART               USART1
#define DEBUG_USART_IRQn          USART1_IRQn

#define DEBUG_USART_TX_PIN        GPIO_PIN_9
#define DEBUG_USART_TX_SOURCEPIN  GPIO_PIN_SOURCE_9
#define DEBUG_USART_TX_PORT       GPIOA
#define DEBUG_USART_TX_GPIO_CLK   RCM_AHB_PERIPH_GPIOA
#define DEBUG_USART_TX_COM_CLK    RCM_APB2_PERIPH_USART1
#define DEBUG_USART_TX_AF         GPIO_AF_PIN1

#define DEBUG_USART_RX_PIN        GPIO_PIN_10
#define DEBUG_USART_RX_SOURCEPIN  GPIO_PIN_SOURCE_10
#define DEBUG_USART_RX_PORT       GPIOA
#define DEBUG_USART_RX_GPIO_CLK   RCM_AHB_PERIPH_GPIOA
#define DEBUG_USART_RX_COM_CLK    RCM_APB2_PERIPH_USART1
#define DEBUG_USART_RX_AF         GPIO_AF_PIN1

/******************************************************************************************/

#define USART_REC_LEN               200         /* 定义最大接收字节数 200 */
#define USART_EN                 1           /* 使能（1）/禁止（0）串口1接收 */

extern uint8_t  g_usart_rx_buf[USART_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart_rx_sta;                 /* 接收状态标记 */

void uart_debug_init(void);
void USART_Write(USART_T* usart, uint8_t* dat);
void USART_Receive_Isr(void);


#endif
