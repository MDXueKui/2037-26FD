#ifndef __SYSTICK_DELAY_H
#define __SYSTICK_DELAY_H

#include "stdint.h"
#include "apm32f0xx.h"
#include "tm1650.h"
#include "input_scan.h"

void delay_us(uint32_t nus);
void delay_nms(uint32_t nms);
void delay_init(void);
uint32_t get_systick_value(void);

#endif
