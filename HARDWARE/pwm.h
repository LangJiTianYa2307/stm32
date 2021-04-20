#ifndef __PWM_H
#define __PWM_H

#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;
void PWM_Init(uint32_t arr, uint32_t psc);
void Set_TIM3_Compare4(uint32_t value);

#endif
