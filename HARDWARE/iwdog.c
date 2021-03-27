#include "iwdog.h"

IWDG_HandleTypeDef IWDG_Handler;

//prer:�������ÿ��Ź��ķ�Ƶֵ 
//rlr:���������Զ�����װ�ص�ֵ
//���㹫ʽ��Tout=((4*2^prer)*rlr)/32 (ms)
//1000ms����һ�Σ�������趨��ʱ����û�����ι����������и�λ
void IWDG_Init(uint32_t prer,uint32_t rlr)
{
	//ʹ�ö������Ź�
	IWDG_Handler.Instance = IWDG;
	IWDG_Handler.Init.Prescaler = prer;
	IWDG_Handler.Init.Reload = rlr;
	IWDG_Handler.Init.Window = IWDG_WINDOW_DISABLE;
	
	HAL_IWDG_Init(&IWDG_Handler);
}

//ι��
void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&IWDG_Handler); //����װ��
}
