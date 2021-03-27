#ifndef __IWDOG_H
#define __IWDOG_H

#include "sys.h"
#include "led.h"

extern WWDG_HandleTypeDef WWDG_Handler;
void 	WWDG_Init(uint32_t cr,uint32_t wr,uint32_t fprer);

#endif
