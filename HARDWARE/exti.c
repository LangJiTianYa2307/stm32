#include "exti.h"

//中断配置和设置
void Exit_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	
	//key0 and key1 Init
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pin = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOH,&GPIO_InitStructure);
	
	//key2 Init
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//key3 Init,上升沿触发
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//设置中断线
	#if KEY_INT
			HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		  HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);
			
			HAL_NVIC_EnableIRQ(EXTI2_IRQn);
		  HAL_NVIC_SetPriority(EXTI2_IRQn,2,1);
			
			HAL_NVIC_EnableIRQ(EXTI3_IRQn);
		  HAL_NVIC_SetPriority(EXTI3_IRQn,2,2);
			
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		  HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,3);
	#endif
}

//编写中断服务函数
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

//编写中断回调函数，进行按键处理
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		switch(GPIO_Pin)
		{
			case GPIO_PIN_0:
				LED0_Toggle;         
				if(LED0_State == GPIO_PIN_SET)
						LED1(0);
				else
						LED1(1);
			  break;
			case GPIO_PIN_2:
				LED1_Toggle;
				break;
			case GPIO_PIN_3:
				LED0_Toggle;
				LED1_Toggle;
				break;
			case GPIO_PIN_13:
				LED0_Toggle;
				break;
			default:
				break;
		}
}
