#include "pwm.h"

TIM_HandleTypeDef TIM3_Handler;
TIM_OC_InitTypeDef TIM3_CH4Handler;

//定时器计算公式：Tout=((arr+1)*(psc+1))/Ft  us
//Ft=定时器的工作频率 单位:Mhz
//example: arr = 5000-1, psc=10800-1 Ft=108000000 那么定时器的周期为0.5s
void PWM_Init(uint32_t arr, uint32_t psc)
{
	TIM3_Handler.Instance = TIM3;
	TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM3_Handler.Init.CounterMode= TIM_COUNTERMODE_UP;
	TIM3_Handler.Init.Period= arr;
	TIM3_Handler.Init.Prescaler= psc;
	HAL_TIM_PWM_Init(&TIM3_Handler);
	
	TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1;
	TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW;
	TIM3_CH4Handler.Pulse=arr/2;
	HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);
}


//外设设置
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//开启时钟
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitStructure.Pin=GPIO_PIN_1;
	GPIO_InitStructure.Speed= GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate=GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//直接设置比较寄存器
void Set_TIM3_Compare4(uint32_t value)
{
	TIM3->CCR4=value;
}
