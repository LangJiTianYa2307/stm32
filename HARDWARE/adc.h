#ifndef _ADC_H
#define _ADC_H

#include "sys.h"
#include "delay.h"

extern ADC_HandleTypeDef ADC_Handler;

void MY_ADC_Init(void);
//获取某一通道的ADC转换结果的值
u16 Get_ADC(u32 ch);
u16 Get_Adc_Average(u32 ch,u8 times);

#endif
