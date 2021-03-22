#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
void Delay(__IO uint32_t nCount);
void Delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}

int main(void)
{
//	GPIO_InitTypeDef GPIO_Initure;

	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				              //初始化HAL库
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
	delay_init(216);                //延时初始化

//	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1; //PB1,0
//	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
//	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
//	//GPIO_Initure.Alternate = GPIO_AF1_UART5;
//	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	while(1)
	{
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB1置1
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//PB0置1
//		Delay(0x1FFFFFF);
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1置0
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);	//PB0置0
//		Delay(0x1FFFFFF);
		  //led control
//		  LED0(0);
//		  LED1(1);
//		  delay_ms(500);
//		  LED0(1);
//		  LED1(0);
//		  delay_ms(500);
		//key control
	  GPIO_KeyValue keyvalue;
		keyvalue = KEY_Scan(0);
		switch(keyvalue)
    {
			case GPIO_KEY_0:
				LED0_Toggle;
			  LED1_Toggle;
				break;
			case GPIO_KEY_1:
				LED1_Toggle;
				break;
			case GPIO_KEY_2:
				LED0_Toggle;
				break;
			case GPIO_KEY_WK_UP:
				LED0_Toggle;         
	      if(LED0_State == GPIO_PIN_SET)
						LED1(0);
				else
					  LED1(1);
				break;
			default:
				break;
		}
	}
}
