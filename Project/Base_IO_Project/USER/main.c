
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"

//STM32F407������ 
//STM32F4����ģ��-�⺯���汾


int main(void)
{
	u32 t=0;
	//uart1_init(115200);
	delay_init(84);
	
  while(1){
    printf("t:%d\r\n",t);
		delay_ms(500);
		t++;
	}
}




