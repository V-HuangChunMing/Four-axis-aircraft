#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "pwm.h"
#include "input_capture.h"
#include "oled.h"
#include "gy86.h"
#include "time.h"
#include "esc.h"

//所有的初始化
void Init_ALL(void);



u32 result=0;

int main(void)
{
	
	Init_ALL();



	while(1)
	{
		


	}
}
	

void Init_ALL(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	
	//time_init
	Clock_Config();
	
	//delay
	delay_init(168);
	
	//BlueTooth
	uart1_init(115200);
	
	//ESC
	TIM1_PWM_Init();
	
	
	//Receiver
	TIM3_CAP_Init();
	
	
	//OLED
	OLED_Init();
	
	
	//MPU
	MPU_Init();
	
	
}
