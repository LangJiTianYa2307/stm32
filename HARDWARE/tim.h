#ifndef __TIM_H
#define __TIM_H

#include "sys.h"
#include "led.h"

extern TIM_HandleTypeDef TIM_Handler;
void TIM3_Init(uint32_t pre,uint32_t arr);


#endif
