#include "sys.h"
#include "delay.h"
#include "usart1.h" 
#include "led.h"
#include "key.h"
#include "exti.h"
#include "iwdog.h"
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

	LED_GPIO_Init();
	//Key_GPIO_Init();
	uart_init(115200);
	Exit_GPIO_Init();
	delay_ms(100);
	IWDG_Init(IWDG_PRESCALER_64,500);
	
  //LED常亮 
	LED0(0);
	
//	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1; //PB1,0
//	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
//	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
//	//GPIO_Initure.Alternate = GPIO_AF1_UART5;
//	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	while(1)
	{
		#if LED_CONTROL
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
		#endif
		#if KEY_CONTROL
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
		#endif
		
		#if USART_CONTROL
		u16 len;
		if(USART_STA & 0x8000)
		{
			len = USART_STA & 0x3fff;
			HAL_UART_Transmit(UART_Handler, (uint8_t *)USART_REC_BUFFER, len, 1000);
			//检测是否发送完毕
			while(__HAL_UART_GET_FLAG(UART_Handler,UART_FLAG_TC));
			USART_STA = 0;
		}
		else
    {
			//进行串口打印处理
		}
		#endif
		
		//按键中断启动
		#if KEY_INT
    printf("OK\n");
		delay_ms(1000);
		#endif
		
		//独立看门狗
		#if IWDOG
		if(KEY_Scan(0) == GPIO_KEY_WK_UP)
		{
			IWDG_Feed();
		}
		delay_ms(10);
		#endif
	}
}
