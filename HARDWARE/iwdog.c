#include "iwdog.h"

IWDG_HandleTypeDef IWDG_Handler;

//prer:用于设置看门狗的分频值 
//rlr:用于设置自动重新装载的值
//计算公式：Tout=((4*2^prer)*rlr)/32 (ms)
//1000ms更新一次，如果在设定的时间内没有完成喂狗，程序进行复位
void IWDG_Init(uint32_t prer,uint32_t rlr)
{
	//使用独立看门狗
	IWDG_Handler.Instance = IWDG;
	IWDG_Handler.Init.Prescaler = prer;
	IWDG_Handler.Init.Reload = rlr;
	IWDG_Handler.Init.Window = IWDG_WINDOW_DISABLE;
	
	HAL_IWDG_Init(&IWDG_Handler);
}

//喂狗
void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&IWDG_Handler); //重新装载
}
