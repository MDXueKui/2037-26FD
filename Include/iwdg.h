#ifndef __IWDG_H
#define __IWDG_H

#include "apm32f0xx_rcm.h"
#include "apm32f0xx_iwdt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IWDT_time   4  /* 看门狗喂狗时间，单位：ms */

void IWDTInit(void);
void IWDT_Feed(void);

#ifdef __cplusplus
}
#endif

#endif
