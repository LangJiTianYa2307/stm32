#include "sys.h"
#include "delay.h"
#include "usart1.h" 
#include "led.h"
#include "key.h"
#include "exti.h"
#include "iwdog.h"
#include "wwdog.h"
#include "tim.h"
#include "pwm.h"
#include "IC.h"
#include "adc.h"

#define PWM 1
#define IC 1
#define ADCV 1
#define TEMPSENSER 1
int direction = 0;
uint32_t CompareValue=0;
void Delay(__IO uint32_t nCount);
void Delay(__IO uint32_t nCount)
{
	while(nCount--)
	{
	}
}

//���ڲ���ߵ�ƽ����ʱ��
extern u8 TIM5CH1_CAPTURE_STA;
extern u32 TIM5CH1_CAPTURE_VAL;

int main(void)
{
//	GPIO_InitTypeDef GPIO_Initure;

	Cache_Enable();                 //��L1-Cache
	
	HAL_Init();				              //��ʼ��HAL��
	Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
	delay_init(216);                //��ʱ��ʼ��

	LED_GPIO_Init();
	//Key_GPIO_Init();
	uart_init(115200);
	Exit_GPIO_Init();
	delay_ms(100);
	IWDG_Init(IWDG_PRESCALER_64,500);
	#if TIM
	//��������Ϊ0.5s�������жϣ�LED1��תһ��
	TIM3_Init(10800,5000);
	#endif
		
	#if PWM
	//����Ƶ��Ϊ2kHZ��Ƶ�ʣ�
	PWM_Init(500-1,108-1);
	//���ڿ��ƱȽϼĴ���ֵ�ļӼ�����
	#endif
	
	#if IC
	TIM5_CH1_Capture_Init(0XFFFFFFFF,108-1);   //��1MHZ��Ƶ�ʼ���
	#endif
	
  //LED���� 
	LED0(0);
	
	
//	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1; //PB1,0
//	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
//	GPIO_Initure.Pull=GPIO_PULLUP;          //����
//	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
//	//GPIO_Initure.Alternate = GPIO_AF1_UART5;
//	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	while(1)
	{
		#if LED_CONTROL
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);	//PB1��1
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//PB0��1
//		Delay(0x1FFFFFF);
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);	//PB1��0
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);	//PB0��0
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
			//����Ƿ������
			while(__HAL_UART_GET_FLAG(UART_Handler,UART_FLAG_TC));
			USART_STA = 0;
		}
		else
    {
			//���д��ڴ�ӡ����
		}
		#endif
		
		//�����ж�����
		#if KEY_INT
    printf("OK\n");
		delay_ms(1000);
		#endif
		
		//�������Ź�
		#if IWDOG
		if(KEY_Scan(0) == GPIO_KEY_WK_UP)
		{
			IWDG_Feed();
		}
		delay_ms(10);
		#endif
		//����TIM
		#if TIM
		LED0_Toggle;
		delay_ms(10);
		#endif
		
		//����PWM
		#if PWM
		if(!direction)
    {
			CompareValue++;
			if(CompareValue>=300)
				direction = 1;
		}
		else
		{
			CompareValue--;
			if(CompareValue<=0)
				direction = 0;
		}
		
		Set_TIM3_Compare4(CompareValue);
		#endif
		
		#if IC
		if(TIM5CH1_CAPTURE_STA&0X80)
    {
			long long temp;
			temp = TIM5CH1_CAPTURE_STA & 0x3f;
			temp *= 0xFFFFFFFF;
      temp += TIM5CH1_CAPTURE_VAL;       
			printf("HIGH:%lld us \r\n",temp);  
			TIM5CH1_CAPTURE_STA = 0;            //������һ�β���
		}
		#endif
		
		#if ADCV
		{
			u16 adcx;
			float  temp;
			adcx =Get_Adc_Average(ADC_CHANNEL_5,20);
			temp= (float)adcx*(3.3/4096);
			printf("The adc convert value is %f\r\n",temp);
		}
		#endif
		
		#if TEMPSENSER 
		{
			u16 adcx;
			double  temp;
			adcx =Get_Adc_Average(ADC_CHANNEL_5,20);
			temp= (float)adcx*(3.3/4096);
			//�����ڲ��¶ȹ�ʽ
			temp = (temp - 0.76) / 0.0025 + 25 ;
			printf("The tempsenser value is %f\r\n",temp);
		}
		#endif
	}
}
