#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pwm.h"
#include "oled.h"


//STM32F407¿ª·¢°å 
//STM32F4¹¤³ÌÄ£°å-¿âº¯Êý°æ±¾


int main(void)
{
	u32 t=0;
	u32 i=0;
	extern const unsigned char BMP1[];
	uart1_init(115200);
	delay_init(84);
	TIM1_PWM_Init();
	Set_PWM_VAL(PWM1,100);
	OLED_Init();

	  
	while(1)
	{
		OLED_Fill(0xFF);//È«ÆÁµãÁÁ
		delay_ms(200);
		OLED_Fill(0x00);//È«ÆÁÃð
		delay_ms(200);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//²âÊÔÏÔÊ¾ÖÐÎÄ
		}
		delay_ms(200);
		OLED_ShowStr(0,3,"HelTec Automation",1);//²âÊÔ6*8×Ö·û
		OLED_ShowStr(0,4,"Hello Tech",2);				//²âÊÔ8*16×Ö·û
		delay_ms(200);
		OLED_CLS();//ÇåÆÁ
		OLED_OFF();//²âÊÔOLEDÐÝÃß
		delay_ms(200);
		OLED_ON();//²âÊÔOLEDÐÝÃßºó»½ÐÑ
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//²âÊÔBMPÎ»Í¼ÏÔÊ¾
		delay_ms(200);
	}
}
	





