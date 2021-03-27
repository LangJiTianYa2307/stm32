#ifndef _USART1_H
#define _USART1_H

#include "sys.h"
#include "stdio.h"

//设置串口缓存大小
#define RXBUFFERSIZE 1
//开启串口接收中断
#define EN_USART1_RX 1
//设置串口最大接收数据长度
#define MAX_USART_DATA_LENGTH 128
//设置数据缓存数组


extern UART_HandleTypeDef UART_Handler;
extern u8 RxBuffer[RXBUFFERSIZE];
extern u8 USART_REC_BUFFER[MAX_USART_DATA_LENGTH];
extern u16 USART_STA;
extern u8 Res;

void uart_init(u32 rate);


#endif
