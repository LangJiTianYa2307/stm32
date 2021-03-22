#ifndef _LED_H
#define _LED_H

#include "sys.h"

#define LED0(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET):\
									HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET))
#define LED0_State (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1))
									
#define LED0_Toggle (HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1))

#define LED1(n) (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET):\
									HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET))
#define LED1_State (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0))
//·­×ªµçÆ½									
#define LED1_Toggle (HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0))

void LED_GPIO_Init(void);


#endif
