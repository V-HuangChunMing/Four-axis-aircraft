#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pwm.h"
#include "oled.h"


//STM32F407������ 
//STM32F4����ģ��-�⺯���汾


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
		OLED_Fill(0xFF);//ȫ������
		delay_ms(200);
		OLED_Fill(0x00);//ȫ����
		delay_ms(200);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//������ʾ����
		}
		delay_ms(200);
		OLED_ShowStr(0,3,"HelTec Automation",1);//����6*8�ַ�
		OLED_ShowStr(0,4,"Hello Tech",2);				//����8*16�ַ�
		delay_ms(200);
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		delay_ms(200);
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		delay_ms(200);
	}
}
	





