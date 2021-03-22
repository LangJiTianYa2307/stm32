#include "key.h"

void Key_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	
	//key0 and key1 Init
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOH,&GPIO_InitStructure);
	
	//key2 Init
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//key3 Init
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

GPIO_KeyValue KEY_Scan(u8 mode)
{
  static u8 key_up = 1;
	if(mode == 1) 
		key_up = 1 ; //按键持续按下
  if(key_up && (KEY0 ==0 || KEY1 ==0 ||KEY2 ==0 || KEY_WK_UP ==1))
	{
		delay_ms(10);
		if(KEY0 == 0) return GPIO_KEY_0;
		if(KEY1 == 0) return GPIO_KEY_1;
		if(KEY2 == 0) return GPIO_KEY_2;
		if(KEY_WK_UP == 1) return GPIO_KEY_WK_UP;
	}
	else if(KEY0 ==1 && KEY1 ==1 && KEY2 ==1 && KEY_WK_UP ==0)
		key_up = 1;
	return GPIO_None;
}
