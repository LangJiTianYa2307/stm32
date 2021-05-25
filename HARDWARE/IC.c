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
	//配置TIM5通道1
	HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH1_Config,TIM_CHANNEL_1);
	
	//开启定时器中断
	HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);
	
	//中断更新使能
	__HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);
	
}

//定时器底层驱动
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
	
	//设置中断优先级
	HAL_NVIC_SetPriority(TIM5_IRQn,2,0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
}

void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if((TIM5CH1_CAPTURE_STA & 0X80) ==0)  //还没有成功捕获
  {
		//捕获到一个下降沿
		if((TIM5CH1_CAPTURE_STA&0X40))
		{
			TIM5CH1_CAPTURE_STA |= 0X80;  //标记成功捕获到一次高电平
			TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);
			TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);  //清楚原来的上升沿捕获设置
			TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);  //设置上升沿捕获
		}
		else
    {
			TIM5CH1_CAPTURE_STA=0; //清空
			TIM5CH1_CAPTURE_VAL = 0;
			TIM5CH1_CAPTURE_STA |= 0X40; //标记捕获到上升沿
			__HAL_TIM_DISABLE(&TIM5_Handler); //关闭定时器，对定时器进行操作
			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);          //设置计数清0
			TIM_RESET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1);  //清楚原来的上升沿捕获设置
			TIM_SET_CAPTUREPOLARITY(&TIM5_Handler,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);
			__HAL_TIM_ENABLE(&TIM5_Handler);
		}
	}
}

//定时器更新(溢出)中断处理回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if((TIM5CH1_CAPTURE_STA&0X80) == 0) //未成功捕获
	{
		if(TIM5CH1_CAPTURE_STA & 0X40)
		{
			if(TIM5CH1_CAPTURE_STA & 0X3F == 0X3F)
			{
				TIM5CH1_CAPTURE_STA |=0X80;  //标记完成一次捕获任务
				TIM5CH1_CAPTURE_VAL = 0XFFFFFFFF;
			}
			else
			{
				TIM5CH1_CAPTURE_STA++;
			}
		}
	}
}

