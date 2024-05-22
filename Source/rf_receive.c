#include "rf_receive.h"
#include "gpio_output.h"

#define id_data_max 10 //学码最大记录个数
uint8_t g_rec_long = 0;
static uint8_t g_data_recling[8] = {0};
static uint8_t g_data_recbuf[8] = {0};
uint8_t g_rec_time = 0;
uint8_t g_rec_timesave = 0;
uint8_t g_rec_save = 0;
uint8_t g_out_data1 = 0;
static uint32_t id_data[id_data_max] = {0};
extern EventGroupHandle_t EventGroupHandler;
#define receive_eventbit_0      (1 << 0)            /* 事件位 */

/**
 * @description: 上电后从0x0800FC00这个flash地址开始
 * 							取之前学习的遥控器ID码
 * @return {*}
 */
void rf_id_data_init(void)
{
    uint8_t i = 0;
	/* Unlock Flash for Clear or Write*/
    FMC_Unlock();

	for(i = 0; i < id_data_max; i++)
	{
		id_data[i] = flash_read_word(flash_eeprom_base + i * 4);
		printf("flash_addr = 0x%08x,flash_id_data = 0x%08x\n", flash_eeprom_base + i * 4, id_data[i]);
	}

	/* Lock Flash*/
    FMC_Lock();
}

/**
 * @description: 射频接收初始化函数
 * 					从flash中读取保存的遥控器ID数据
 * 					使用timer3的输入捕获功能对高电平时间进行捕获分析
 * 					48分频，1us计数一次，向上计数模式
 * @return {*}
 */
void rf_receive_Init(void)
{
	rf_id_data_init();
    TMR_TimeBase_T  timeBaseConfig;
    TMR_ICConfig_T  ICconfigstruct;
    GPIO_Config_T   gpioconfig;
    /* Enable Clock*/
    RCM_EnableAHBPeriphClock(data_gpio_clock);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);
    RCM_EnableAPB1PeriphClock(data_tim_clock);

    /*  Connect TMR3 to CH2 */
    GPIO_ConfigPinAF(data_gpio_port, data_gpio_source_pin, data_AF_pin);
    gpioconfig.mode = GPIO_MODE_AF;
    gpioconfig.outtype = GPIO_OUT_TYPE_PP;
    gpioconfig.pin  =  data_gpio_pin;
    gpioconfig.pupd = GPIO_PUPD_NO;
    gpioconfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(data_gpio_port, &gpioconfig);

    /* Set clockDivision = 1 */
    timeBaseConfig.clockDivision =  TMR_CKD_DIV1;
    /* Up-counter */
    timeBaseConfig.counterMode =  TMR_COUNTER_MODE_UP;
    /* Set divider = 0 .So TMR1 clock freq ~= 48/( 95 + 1) = 0.5MHZ */
    timeBaseConfig.div = 47 ;
    /* Set counter = 0xffff */
    timeBaseConfig.period = 0xFFFF;
    /* Repetition counter = 0x0 */
    timeBaseConfig.repetitionCounter =  0;
    TMR_ConfigTimeBase(TMR3, &timeBaseConfig);

    /* Connect InputCapture channel to TMR1_CHANNEL_3*/
    ICconfigstruct.channel  = data_input_channel;
    /*  Set Input Capture filter  */
    ICconfigstruct.ICfilter = 0x00;
    /*  Set Input Capture polarity  */
    ICconfigstruct.ICpolarity  = TMR_IC_POLARITY_BOTHEDGE;
    /*  Set Input Capture divider  */
    ICconfigstruct.ICprescaler = TMR_ICPSC_DIV1;
    /*  Set Input Capture selection  */
    ICconfigstruct.ICselection = TMR_IC_SELECTION_DIRECT_TI;
    TMR_ICConfig(TMR3, &ICconfigstruct);

	/* enable the timer interrupt */
	TMR_EnableInterrupt(TMR3, TMR_INT_CH2);
	NVIC_EnableIRQRequest(TMR3_IRQn, 1);

    // /*  Enable TMR3  */
    TMR_Enable(TMR3);
}

/**
 * @brief       定时器3中断服务函数，负责接收处理rf信号
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void TMR3_IRQHandler(void)
{
	if(TMR_ReadIntFlag(TMR3, TMR_INT_FLAG_CH2) == SET)
	{
		uint32_t data_time = 0;
		data_time = TMR_ReadCaputer2(TMR3);
		static uint8_t rec_ing = 0;
        BaseType_t xHigherPriorityTaskWoken, xResult;
		/* 禹顺协议，允许±100us的偏差 */
		if((316 <= data_time )&& (data_time <= 516))
		{
			if(!GPIO_ReadInputBit(data_gpio_port, data_gpio_pin))
			{
				if(!rec_ing)
				{
					rec_ing=1;
					g_data_recling[g_rec_long/8]|=(1<<(7-(g_rec_long%8)));
					++g_rec_long;
					TMR_SetCounter(TMR3, 0);	// 下降沿时将定时器清0
				}
			}
		}
		else if((732 <= data_time) && (data_time <= 932))
		{
			if(GPIO_ReadInputBit(data_gpio_port, data_gpio_pin))
			{
				g_data_recling[g_rec_long/8]&=~(1<<(7-(g_rec_long%8)));
			}
			else
			{
				g_data_recling[g_rec_long/8]|=(1<<(7-(g_rec_long%8)));
			}
			++g_rec_long;

			if(g_rec_long == 56)
			{
//				g_rec_fg = 1;
				for(uint8_t i = 0; i < 7; i++)
				{
//					printf("the data is 0x%02x\n", g_data_recling[i]);
					g_data_recbuf[i] = g_data_recling[i];
				}
                g_rec_timesave = g_rec_time;
				g_rec_time = 0;
                xHigherPriorityTaskWoken = pdFALSE;
                xResult = xEventGroupSetBitsFromISR(EventGroupHandler, receive_eventbit_0, &xHigherPriorityTaskWoken);
//                printf("xResult = %ld\r\n", xResult);
                /* 信息是否发送成功 */
                if ( xResult != pdFAIL )
                {
                    /* 如果 xHigherPriorityTaskWoken 的值为 pdTRUE 则进行一次上下文切换*/
                    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
                }
			}
			TMR_SetCounter(TMR3, 0);
		}
		else
		{
			TMR_SetCounter(TMR3, 0);
			rec_ing = 0;
			g_rec_long = 0;
		}
		TMR_ClearIntFlag(TMR3, TMR_INT_FLAG_CH2);
	}
}
/**
 * @description: 学码函数，最多记忆10个码，如果超过则覆盖第一个码
 *
 * @param {uint32_t} id_dataing
 * @return {*}
 */
void learn_idcode(uint32_t id_dataing)
{
	uint8_t i = 0;
	/* 遍历数组，找到哪个位置还没有存储ID码 */
	for(i = 0; i < id_data_max; i++)
	{
		if(id_data[i] == id_dataing)
		{
			break;
		}
		else if(id_data[i] == 0xffffffff)
		{
			id_data[i] = id_dataing;
			flash_write_word(flash_eeprom_base + i * 4, id_data[i]);
			printf("flash_id_data = 0x%08x\n", flash_read_word(flash_eeprom_base + i * 4));
			break;
		}
	}
	/* 如果学满了，那么复写第一个id码，然后将所有数据重新写入flash中 */
	if(i == id_data_max)
	{
		FMC_Unlock();
		FMC_ErasePage(flash_eeprom_base);
		FMC_Lock();
		id_data[0] = id_dataing;
		for(i = 0; i < id_data_max; i++)
		{
			flash_write_word(flash_eeprom_base + i * 4, id_data[i]);
		}
	}
	set_learn_eventbit_1(0);
	buzzer_quick_3s();
}

/**
 * @description: 删码函数，由于删除必须要页删除，所以先删除整页数据
 * 然后在本地将待删数据删除后将剩余数据复写到flash中
 * @param {uint32_t} id_dataing
 * @return {*}
 */
void delete_idcode(uint32_t id_dataing)
{
	FMC_Unlock();
	FMC_ErasePage(flash_eeprom_base);
	FMC_Lock();
	uint8_t i = 0;
	for(i = 0; i < id_data_max; i++)
	{
		if(id_data[i] == id_dataing)
		{
			id_data[i] = 0xffffffff;
		}
	}
	for(i = 0; i < id_data_max; i++)
	{
		flash_write_word(flash_eeprom_base + i * 4, id_data[i]);
	}
	set_learn_eventbit_1(0);
	buzzer_quick_3s();
}

void rf_data_pro(void)
{
	switch(g_out_data1)
	{
		case 0x01:
			buzzer_quick_100ms();
            if(get_lock_eventbit12)
            {
                vTaskDelay(10);
            }
            else
            {
                if(tube_display_buff[0] == 1)
                {
                    motor_up();
                }
                else if(tube_display_buff[0] == 5 && ManualAuto_flag &&\
                    (GPIO_ReadOutputBit(firefeedback_output_PORT, firefeedback_output_GPIO_PIN)))
                {
                    motor_up();
                    BUZZER(1);
                    set_fire_eventbit7(1);
                }
                else if(tube_display_buff[0] == 6 && (!get_fire_eventbit7))
                {
                    motor_up();
                }
                else
                {
                    motor_on_all();
                }
            }
			break;
		case 0x02:
			buzzer_quick_100ms();
            if(get_lock_eventbit12)
            {
                vTaskDelay(10);
            }
            else
            {
                if(tube_display_buff[0] == 1)
                {
                    motor_stop();
                }
                else if(tube_display_buff[0] == 6 && (!get_fire_eventbit7))
                {
                    motor_stop();
                }
                else
                {
                    stop_motor();
                }
            }
			break;
		case 0x04:
            buzzer_quick_100ms();
            if(get_lock_eventbit12)
            {
                vTaskDelay(10);
            }
            else
            {
                if(tube_display_buff[0] == 1)
                {
                    motor_down();
                }
                else if(tube_display_buff[0] == 6 && (!get_fire_eventbit7))
                {
                    motor_down();
                }
                else
                {
                    motro_off_all();
                }
            }
			break;
		case 0x0f:
            if(get_lock_eventbit12)
            {
                vTaskDelay(10);
            }
            else
            {
                if(tube_display_buff[0] == 1)
                {
                    buzzer_quick_100ms();
                    motor_stop();
                }
            }
		default:
			break;
	}
}


/**
 * @description: rf接收处理任务，当接收完一帧数据后进行处理
 * @return {*}
 */
void Rf_Rec_Pro_Task(void *parameter)
{
    printf("Rf_Rec_Pro_Task start!\r\n");
    while(1)
    {
        xEventGroupWaitBits((EventGroupHandle_t )EventGroupHandler, /* 等待的事件标志组句柄 */
                            (EventBits_t        )receive_eventbit_0,      /* 等待的事件 */
                            (BaseType_t         )pdTRUE,            /* 函数退出时清零等待的事件 */
                            (BaseType_t         )pdTRUE,            /* 等待等待的事件中的所有事件 */
                            (TickType_t         )portMAX_DELAY);    /* 等待时间 */
        uint8_t i = 0;
        uint8_t crc = 0;
        uint32_t id_dataing = 0x00;
        id_dataing = (g_data_recbuf[2] << 24) + (g_data_recbuf[3] << 16) + (g_data_recbuf[4] << 8);
        // printf("id_dataing = 0x%08x\n", id_dataing);
        if((g_data_recbuf[1] == 0xe5))
        {
            if(g_data_recbuf[5] == 0x11 && get_learn_eventbit_1)
            {
                learn_idcode(id_dataing);
            }
            else if (g_data_recbuf[5] == 0x14 && get_learn_eventbit_1)
            {
                delete_idcode(id_dataing);
            }
            else
            {
                for(i = 0; i < id_data_max; i++)
                {
                    if(id_dataing == id_data[i])
                    {
                        /* 会接收三帧数据，当接收到一帧就将引导码更改，阻止校验
                         * 防止单片机重复响应改命令
                         */
                        if(g_data_recbuf[5] == g_rec_save)
                        {
                            if(g_rec_timesave<60)
                            {
                                g_data_recbuf[1] = 0xaa;
                            }
                        }
                        else
                        {
                            g_rec_save = g_data_recbuf[5];
                        }
                        for(i = 0; i < 7; i++)
                        {
                            crc ^= g_data_recbuf[i];
                        }
                        if(!crc)
                        {
                            g_out_data1 = g_data_recbuf[5] & 0x0f;
                            // printf("data = 0x%02x\r\n", g_out_data1);
                            rf_data_pro();
                        }
                        break;
                    }
                }
            }
        }
    }
}
