#include "btim.h"
#include "FreeRTOS.h"

uint32_t FreeRTOSRunTimeTicks;          /* FreeRTOS时间统计所用的节拍计数器 */

void ConfigureTimeForRunTimeStats(void)
{
    FreeRTOSRunTimeTicks = 0;           /* 节拍计数器初始化为0 */
    btim_tim6_int_init(10-1, 480-1);    /* 初始化TIM6 */
}

void btim_tim6_int_init(uint16_t arr, uint16_t psc)
{
     TMR_TimeBase_T  timeBaseConfig;

    /* Enable Clock */
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_TMR6);

    /* Set clockDivision = 1 */
    timeBaseConfig.clockDivision =  TMR_CKD_DIV1;
    /* Up-counter */
    timeBaseConfig.counterMode =  TMR_COUNTER_MODE_UP;
    /* Set divider = 47.So TMR1 clock freq ~= 48/(47 + 1) = 1MHZ */
    timeBaseConfig.div = psc ;
    /* Set counter = 0xffff */
    timeBaseConfig.period = arr;
    /* Repetition counter = 0x0 */
    timeBaseConfig.repetitionCounter =  0;

    TMR_ConfigTimeBase(TMR6, &timeBaseConfig);

    /* Enable update interrupt*/
    TMR_EnableInterrupt(TMR6, TMR_INT_UPDATE);
    NVIC_EnableIRQRequest(TMR6_IRQn, 2);

    /*  Enable TMR14  */
    TMR_Enable(TMR6);
}

/*!
 * @brief       TMR14 interrupt service runtine
 *
 * @param       None
 *
 * @retval      None
 *
 * @note        This function need to put into TMR14_IRQHandler() in apm32f0xx_int.c
 */
void TMR6_Isr(void)
{
    if (TMR_ReadIntFlag(TMR6, TMR_INT_FLAG_UPDATE) == SET)
    {
        TMR_ClearIntFlag(TMR6, TMR_INT_FLAG_UPDATE);

        FreeRTOSRunTimeTicks++;
    }
}









