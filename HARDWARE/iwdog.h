#ifndef __IWDOG_H
#define __IWDOG_H

#include "sys.h"

extern IWDG_HandleTypeDef IWDG_Handler;
void IWDG_Init(uint32_t prer,uint32_t rlr);
void IWDG_Feed(void);


#endif
