#ifndef _IC_H
#define _IC_H

#include "sys.h"

extern TIM_HandleTypeDef TIM5_Handler;
extern u8 TIM5CH1_CAPTURE_STA;
extern u32 TIM5CH1_CAPTURE_VAL;

void TIM5_CH1_Capture_Init(uint32_t arr,uint16_t psc);

#endif
