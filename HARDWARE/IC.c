#include "IC.h"

u8 TIM5CH1_CAPTURE_STA;
u32 TIM5CH1_CAPTURE_VAL;
TIM_HandleTypeDef TIM5_Handler;

void TIM5_CH1_Capture_Init(uint32_t arr,uint16_t psc)
{
	
	TIM_IC_InitTypeDef TIM5_CH1_Config;
	
	TIM5_Handler.Instance = TIM5;
	TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM5_Handler.Init.Period = arr;
	TIM5_Handler.Init.Prescaler = psc;
	
	TIM5_CH1_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM5_CH1_Config.ICFilter = 0;
	TIM5_CH1_Config.ICPrescaler = TIM_ICPSC_DIV1;
	TIM5_CH1_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_IC_Init(&TIM5_Handler);
	//����TIM5ͨ��1
	HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH1_Config,TIM_CHANNEL_1);
	
	//������ʱ���ж�
	HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);
	
	//�жϸ���ʹ��
	__HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);
	
}

//��ʱ���ײ�����
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_TIM5_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM5;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//�����ж����ȼ�
	HAL_NVIC_SetPriority(TIM5_IRQn,2,0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}

void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if((TIM5CH1_CAPTURE_STA & 0X80) ==0)  //��û�гɹ�����
  {
		//����һ���½���
		if((TIM5CH1_CAPTURE_STA&0X40))
		{
			TIM5CH1_CAPTURE_STA |= 0X80;  //��ǳɹ�����һ�θߵ�ƽ
			TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);
			TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);  //���ԭ���������ز�������
			TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);  //���������ز���
		}
		else
    {
			TIM5CH1_CAPTURE_STA=0; //���
			TIM5CH1_CAPTURE_VAL = 0;
			TIM5CH1_CAPTURE_STA |= 0X40; //��ǲ���������
			__HAL_TIM_DISABLE(&TIM5_Handler); //�رն�ʱ�����Զ�ʱ�����в���
			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);          //���ü�����0
			TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);  //���ԭ���������ز�������
			TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);
			__HAL_TIM_ENABLE(&TIM5_Handler);
		}
	}
}

//��ʱ������(���)�жϴ���ص�����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if((TIM5CH1_CAPTURE_STA&0X80) == 0) //δ�ɹ�����
	{
		if(TIM5CH1_CAPTURE_STA & 0X40)
		{
			if(TIM5CH1_CAPTURE_STA & 0X3F == 0X3F)
			{
				TIM5CH1_CAPTURE_STA |=0X80;  //������һ�β�������
				TIM5CH1_CAPTURE_VAL = 0XFFFFFFFF;
			}
			else
			{
				TIM5CH1_CAPTURE_STA++;
			}
		}
	}
}

