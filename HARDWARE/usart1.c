#include "usart1.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/6/23
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

UART_HandleTypeDef UART_Handler;
u8 RxBuffer[RXBUFFERSIZE];
u8 USART_REC_BUFFER[MAX_USART_DATA_LENGTH];
//ʹ�õ�15λ���ڱ�ʾ������ɣ���14Ϊ���ڽ��յ���־
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
	
	//���ж�ģʽ��������
	HAL_UART_Receive_IT(&UART_Handler,(u8 *)RxBuffer,1);
	
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(huart->Instance == USART1)
  {
		//����ʱ��
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

//�жϷ�����,�Ƿ�������ϵͳ
void USART1_IRQHandler(void)
{
	//��������ж�ϵͳ����б���
	#if SYSTEM_SUPPORT_OS
		OSIntEnter();
	#endif
	//��ѯ���ڽ����ж�
	if(__HAL_UART_GET_FLAG(&UART_Handler,UART_FLAG_RXNE) !=RESET)
	{
		//�жϴ�������ʼ��������
		HAL_UART_Receive(&UART_Handler,&Res,1,1000);
		if((USART_STA & 0x8000) == 0)
		{
			//���յ���0x0d
			if((USART_STA & 0x4000) == 0)
			{
			   if(Res != 0x0a) USART_STA = 0;
				 else
					 USART_STA |= 0x8000;
			}
			else
      {
				//������յ�0x0d������Ӧ��־λ��14λ����Ϊ1
				if(Res == 0x0d)  USART_STA |= 0x4000;
				else
				{
					//û���յ�0x0d�����ݻ��浽���鵱��
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

