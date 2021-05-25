#include "usart1.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/6/23
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

UART_HandleTypeDef UART_Handler;
u8 RxBuffer[RXBUFFERSIZE];
u8 USART_REC_BUFFER[MAX_USART_DATA_LENGTH];
//使用第15位用于表示测试完成，第14为用于接收到标志
u16 USART_STA;
u8 Res;

void uart_init(u32 rate)
{
	UART_Handler.Instance = USART1;
	UART_Handler.Init.BaudRate = rate;
	UART_Handler.Init.WordLength  = UART_WORDLENGTH_8B;
	UART_Handler.Init.StopBits = UART_STOPBITS_1;
	UART_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART_Handler.Init.Parity = UART_PARITY_NONE;
	UART_Handler.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UART_Handler);
	
	//在中断模式接收数据
	HAL_UART_Receive_IT(&UART_Handler,(u8 *)RxBuffer,1);
	
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(huart->Instance == USART1)
  {
		//开启时钟
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStructure.Alternate = GPIO_AF7_USART1;
		
		HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
		
		#if EN_USART1_RX
			HAL_NVIC_EnableIRQ(USART1_IRQn);
		  HAL_NVIC_SetPriority(USART1_IRQn,3,3);
		#endif
		
	}
}

//中断服务函数,是否开启操作系统
void USART1_IRQHandler(void)
{
	//如果进入中断系统则进行保护
	#if SYSTEM_SUPPORT_OS
		OSIntEnter();
	#endif
	//查询串口接收中断
	if(__HAL_UART_GET_FLAG(&UART_Handler,UART_FLAG_RXNE) !=RESET)
	{
		//中断触发，开始接收数据
		HAL_UART_Receive(&UART_Handler,&Res,1,1000);
		if((USART_STA & 0x8000) == 0)
		{
			//接收到了0x0d
			if((USART_STA & 0x4000) == 0)
			{
			   if(Res != 0x0a) USART_STA = 0;
				 else
					 USART_STA |= 0x8000;
			}
			else
      {
				//如果接收到0x0d，将对应标志位第14位设置为1
				if(Res == 0x0d)  USART_STA |= 0x4000;
				else
				{
					//没接收到0x0d将数据缓存到数组当中
					USART_REC_BUFFER[USART_STA & 0x3fff] = Res;
					USART_STA++;
					if(USART_STA > (MAX_USART_DATA_LENGTH - 1)) USART_STA=0;
				}
			}
		}
		HAL_UART_IRQHandler(&UART_Handler);
	}
	#if SYSTEM_SUPPORT_OS 
		OSIntExit();
	#endif
}

