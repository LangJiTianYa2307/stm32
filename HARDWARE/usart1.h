#ifndef _USART1_H
#define _USART1_H

#include "sys.h"
#include "stdio.h"

//���ô��ڻ����С
#define RXBUFFERSIZE 1
//�������ڽ����ж�
#define EN_USART1_RX 1
//���ô������������ݳ���
#define MAX_USART_DATA_LENGTH 128
//�������ݻ�������


extern UART_HandleTypeDef UART_Handler;
extern u8 RxBuffer[RXBUFFERSIZE];
extern u8 USART_REC_BUFFER[MAX_USART_DATA_LENGTH];
extern u16 USART_STA;
extern u8 Res;

void uart_init(u32 rate);


#endif
