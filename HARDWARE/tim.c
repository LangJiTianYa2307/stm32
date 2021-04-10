#include "tim.h"

TIM_HandleTypeDef TIM_Handler;

//��ʱ�����㹫ʽ��Tout=((arr+1)*(psc+1))/Ft  us
//Ft=��ʱ���Ĺ���Ƶ�� ��λ:Mhz
//example: arr = 5000-1, psc=10800-1 Ft=108000000 ��ô��ʱ��������Ϊ0.5s
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

//��ʱ����ʼ���ص������������п�����ʱ�ӣ������������ж����ȼ�
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//����ʱ��
	__HAL_RCC_TIM3_CLK_ENABLE();
	//�����ж�
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);
}

//��д�жϷ�����
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
