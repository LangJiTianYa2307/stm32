#include "wwdog.h"

WWDG_HandleTypeDef WWDG_Handler;

//Twwdg=(4096*2^WDGTB*(T[5:0]+1))/Fpclk1;

void 	WWDG_Init(uint32_t cr,uint32_t wr,uint32_t fprer)
{
	WWDG_Handler.Instance = WWDG;
	WWDG_Handler.Init.Counter = cr;
	WWDG_Handler.Init.EWIMode = WWDG_EWI_ENABLE;
	WWDG_Handler.Init.Prescaler = fprer;
	WWDG_Handler.Init.Window = wr;
	HAL_WWDG_Init(&WWDG_Handler);
}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
	//�������Ź�ʱ�Ӻ�����ʱ�����ȼ�
	__HAL_RCC_WWDG_CLK_ENABLE();

		HAL_NVIC_EnableIRQ(WWDG_IRQn);
		HAL_NVIC_SetPriority(WWDG_IRQn,2,3);	
}


//��д�жϷ�����
void WWDG_IRQHandler(void)
{
	HAL_WWDG_IRQHandler(&WWDG_Handler);
}

//ִ�лص�����
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
	HAL_WWDG_Refresh(&WWDG_Handler);
	LED1_Toggle;
}

