#include "adc.h"

ADC_HandleTypeDef ADC_Handler;


//初始化ADC
void MY_ADC_Init(void)
{
	ADC_Handler.Instance = ADC1;
	ADC_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC_Handler.Init.Resolution = ADC_RESOLUTION12b;
	ADC_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_Handler.Init.ScanConvMode = DISABLE;
	ADC_Handler.Init.EOCSelection = DISABLE;
	ADC_Handler.Init.ContinuousConvMode = DISABLE;
	ADC_Handler.Init.NbrOfConversion = 1;
	ADC_Handler.Init.DiscontinuousConvMode = DISABLE;
	ADC_Handler.Init.NbrOfDiscConversion = 0;
	ADC_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_Handler.Init.DMAContinuousRequests = DISABLE;
	
	HAL_ADC_Init(&ADC_Handler);
}

//ADC底层驱动，引脚配置，时钟使能
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}

//获得ADC值
u16 Get_ADC(u32 ch)
{
	ADC_ChannelConfTypeDef ADC_ChanConf;

	ADC_ChanConf.Channel = ch;
	ADC_ChanConf.Rank =1;
	ADC_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC_ChanConf.Offset = 0;
	
	HAL_ADC_ConfigChannel(&ADC_Handler,&ADC_ChanConf);
	
	HAL_ADC_Start(&ADC_Handler);
	HAL_ADC_PollForConversion(&ADC_Handler,10);
	return (u16)HAL_ADC_GetValue(&ADC_Handler);
}

//求取均值
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val = 0;
	int i;
	for(i=0;i<times;i++)
  {
		temp_val += Get_ADC(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
