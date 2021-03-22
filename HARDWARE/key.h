#ifndef _KEY_H
#define _KEY_H

#include "sys.h"
#include "delay.h"

typedef enum
{
	GPIO_None = 0,
  GPIO_KEY_0,
  GPIO_KEY_1,
	GPIO_KEY_2,
	GPIO_KEY_WK_UP
}GPIO_KeyValue;

//key0 PH3 key1 PH2 key2 PC13 key_wk_up PA0
#define KEY0 (HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3))
#define KEY1 (HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2))
#define KEY2 (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13))
#define KEY_WK_UP (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0))

void Key_GPIO_Init(void);
GPIO_KeyValue KEY_Scan(u8 mode);

#endif
