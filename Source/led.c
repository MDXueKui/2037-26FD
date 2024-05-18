#include "led.h"

void led_init(void)
{
    GPIO_Config_T gpioConfig;

    RCM_EnableAHBPeriphClock(LED2_GPIO_CLK | LED4_GPIO_CLK | LED8_GPIO_CLK);
    /* LED2 GPIO configuration */
    gpioConfig.pin = LED1_GPIO_PIN;
    gpioConfig.mode = GPIO_MODE_OUT;
    gpioConfig.outtype = GPIO_OUT_TYPE_PP;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    gpioConfig.pupd = GPIO_PUPD_NO;
    GPIO_Config(LED1_GPIO_PORT, &gpioConfig);

    /* LED2 GPIO configuration */
    gpioConfig.pin = LED2_GPIO_PIN;
    GPIO_Config(LED2_GPIO_PORT, &gpioConfig);

    /* LED3 GPIO configuration */
    gpioConfig.pin = LED3_GPIO_PIN;
    GPIO_Config(LED3_GPIO_PORT, &gpioConfig);

    /* LED4 GPIO configuration */
    gpioConfig.pin = LED4_GPIO_PIN;
    GPIO_Config(LED4_GPIO_PORT, &gpioConfig);

    /* LED8 GPIO configuration */
    gpioConfig.pin = LED8_GPIO_PIN;
    GPIO_Config(LED8_GPIO_PORT, &gpioConfig);
    
    /* LED2 GPIO configuration */
    gpioConfig.pin = LEDTEST_GPIO_PIN;
    GPIO_Config(LEDTEST_GPIO_PORT, &gpioConfig);
    
//    LED1(0);
//    LED2(0);
//    LED3(0);
//    LED4(0);
//    LED8(0);
    LEDTEST(0);
}

void GPIO_TogglePin(GPIO_T* port, uint32_t pin)
{
    port->ODATA ^= pin;
}
