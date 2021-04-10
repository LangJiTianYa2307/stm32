#include "tim.h"

TIM_HandleTypeDef TIM_Handler;

//定时器计算公式：Tout=((arr+1)*(psc+1))/Ft  us
//Ft=定时器的工作频率 单位:Mhz
//example: arr = 5000-1, psc=10800-1 Ft=108000000 那么定时器的周期为0.5s
void TIM3_Init(uint32_t pre,uint32_t arr)
{
	TIM_Handler.Instance = TIM3;
	TIM_Handler.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;
	TIM_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Handler.Init.Period = arr;
	TIM_Handler.Init.Prescaler=pre;
	
	HAL_TIM_Base_Init(&TIM_Handler);
	
	HAL_TIM_Base_Start_IT(&TIM_Handler);
}

//定时器初始化回调函数，在其中开启了时钟，并且设置了中断优先级
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//开启时钟
	__HAL_RCC_TIM3_CLK_ENABLE();
	//设置中断
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);
}

//编写中断服务函数
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_Handler);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		LED1_Toggle;
	}
}
