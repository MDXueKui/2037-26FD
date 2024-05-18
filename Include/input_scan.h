#ifndef __INPUT_SCAN_H
#define __INPUT_SCAN_H

#include "apm32f0xx_gpio.h"
#include "apm32f0xx_rcm.h"
#include "stdint.h"
#include "tm1650.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


#define ADD_PIN                  GPIO_PIN_2
#define ADD_GPIO_PORT            GPIOB
#define ADD_KEY                  GPIO_ReadInputBit(ADD_GPIO_PORT, ADD_PIN)
#define ADD_GPIO_CLK             RCM_AHB_PERIPH_GPIOB
#define ADD_PRES                 0X1

#define SBU_PIN                  GPIO_PIN_10
#define SBU_GPIO_PORT            GPIOB
#define SBU_KEY                  GPIO_ReadInputBit(SBU_GPIO_PORT, SBU_PIN)
#define SBU_GPIO_CLK             RCM_AHB_PERIPH_GPIOB
#define SBU_PRES                 0X2

#define RESET_KEY_PIN            GPIO_PIN_11
#define RESET_KEY_PORT           GPIOB
#define RESET_KEY                GPIO_ReadInputBit(RESET_KEY_PORT, RESET_KEY_PIN)
#define RESET_KEY_GPIO_CLK       RCM_AHB_PERIPH_GPIOB
#define RESET_KEY_PRES           0X3

#define SET_PIN                  GPIO_PIN_15
#define SET_GPIO_PORT            GPIOC
#define SET_KEY                  GPIO_ReadInputBit(SET_GPIO_PORT, SET_PIN)
#define SET_GPIO_CLK             RCM_AHB_PERIPH_GPIOC
#define SET_PRES                 0X4

#define ON_PIN                   GPIO_PIN_6
#define ON_GPIO_PORT             GPIOB
#define ON_KEY                   GPIO_ReadInputBit(ON_GPIO_PORT, ON_PIN)
#define ON_GPIO_CLK              RCM_AHB_PERIPH_GPIOB
#define ON_PRES                  0X5

#define OFF_PIN                  GPIO_PIN_4
#define OFF_GPIO_PORT            GPIOB
#define OFF_KEY                  GPIO_ReadInputBit(OFF_GPIO_PORT, OFF_PIN)
#define OFF_GPIO_CLK             RCM_AHB_PERIPH_GPIOB
#define OFF_PRES                 0X6

#define Controlled_input_PIN                       GPIO_PIN_8
#define Controlled_input_GPIO_PORT                 GPIOA
#define Controlled_input_KEY                       GPIO_ReadInputBit(Controlled_input_GPIO_PORT, Controlled_input_PIN)
#define Controlled_input_GPIO_CLK                  RCM_AHB_PERIPH_GPIOB
#define Controlled_input_Triggering                0X7

#define fire_control_input_PIN                     GPIO_PIN_12
#define fire_control_input_GPIO_PORT               GPIOB
#define fire_control_input_KEY                     GPIO_ReadInputBit(fire_control_input_GPIO_PORT, fire_control_input_PIN)
#define fire_control_input_GPIO_CLK                RCM_AHB_PERIPH_GPIOB
#define fire_control_input_Triggering              0X8

#define fire_control24v_input_PIN                  GPIO_PIN_13
#define fire_control24v_input_GPIO_PORT            GPIOB
#define fire_control24v_input_KEY                  GPIO_ReadInputBit(fire_control24v_input_GPIO_PORT, fire_control24v_input_PIN)
#define fire_control24v_input_GPIO_CLK             RCM_AHB_PERIPH_GPIOB
#define fire_control24v_input_Triggering           0X9

#define cascaded24vR_input_PIN                      GPIO_PIN_15
#define cascaded24vR_input_GPIO_PORT                GPIOB
#define cascaded24vR_input_KEY                      GPIO_ReadInputBit(cascaded24vR_input_GPIO_PORT, cascaded24vR_input_PIN)
#define cascaded24vR_input_GPIO_CLK                 RCM_AHB_PERIPH_GPIOB
#define cascaded24vR_input_Triggering               0X10

#define cascaded24vL_input_PIN                      GPIO_PIN_11
#define cascaded24vL_input_GPIO_PORT                GPIOA
#define cascaded24vL_input_KEY                      GPIO_ReadInputBit(cascaded24vL_input_GPIO_PORT, cascaded24vL_input_PIN)
#define cascaded24vL_input_GPIO_CLK                 RCM_AHB_PERIPH_GPIOA
#define cascaded24vL_input_Triggering               0X11

#define rain_input_PIN                             GPIO_PIN_12
#define rain_input_GPIO_PORT                       GPIOA
#define rain_input_KEY                             GPIO_ReadInputBit(rain_input_GPIO_PORT, rain_input_PIN)
#define rain_input_GPIO_CLK                        RCM_AHB_PERIPH_GPIOA
#define rain_input_Triggering                      0X12

#define up2_input_PIN                               GPIO_PIN_6
#define up2_input_GPIO_PORT                         GPIOF
#define up2_input_KEY                               GPIO_ReadInputBit(up2_input_GPIO_PORT, up2_input_PIN)
#define up2_input_GPIO_CLK                          RCM_AHB_PERIPH_GPIOF
#define up2_input_Triggering                        0X13

#define down2_input_PIN                             GPIO_PIN_7
#define down2_input_GPIO_PORT                       GPIOF
#define down2_input_KEY                             GPIO_ReadInputBit(down2_input_GPIO_PORT, down2_input_PIN)
#define down2_input_GPIO_CLK                        RCM_AHB_PERIPH_GPIOF
#define down2_input_Triggering                      0X14

#define up1_input_PIN                               GPIO_PIN_15
#define up1_input_GPIO_PORT                         GPIOA
#define up1_input_KEY                               GPIO_ReadInputBit(up1_input_GPIO_PORT, up1_input_PIN)
#define up1_input_GPIO_CLK                          RCM_AHB_PERIPH_GPIOA
#define up1_input_Triggering                        0X15

#define down1_input_PIN                             GPIO_PIN_3
#define down1_input_GPIO_PORT                       GPIOB
#define down1_input_KEY                             GPIO_ReadInputBit(down1_input_GPIO_PORT, down1_input_PIN)
#define down1_input_GPIO_CLK                        RCM_AHB_PERIPH_GPIOB
#define down1_input_Triggering                      0X16

#define MOSI_PIN                                    GPIO_PIN_7
#define MOSI_GPIO_PORT                              GPIOB
#define MOSI_KEY                                    GPIO_ReadInputBit(MOSI_GPIO_PORT, MOSI_PIN)
#define MOSI_GPIO_CLK                               RCM_AHB_PERIPH_GPIOB
#define MOSI_Triggering                             0X17


void input_scan_init(void);
void Input_Scan_Task (void* parameter);

#endif
