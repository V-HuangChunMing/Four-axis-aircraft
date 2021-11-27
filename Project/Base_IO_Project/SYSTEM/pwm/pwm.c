#include "pwm.h"
#define PSC 167//时钟预分频数
#define ARR 4999//自动重装值
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); APB2的频率为168Mhz




void TIM1_PWM_Init(void)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE时钟
	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); //GPIOE9复用为定时器1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;           //GPIOFA	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE9

	TIM_TimeBaseStructure.TIM_Prescaler=PSC;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=ARR;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1

	//初始化TIM1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM3
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//使能pWM输出
	
	TIM_SetCompare1(TIM1,0);
	TIM_SetCompare2(TIM1,0);
	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare4(TIM1,0);
	
	
	

//	//此部分需手动修改IO口设置
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM3时钟使能    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTA时钟	
//	
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOA6复用为定时器3
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOFA
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
//	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PA6
//	  
//	TIM_TimeBaseStructure.TIM_Prescaler=PSC;  //定时器分频
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseStructure.TIM_Period=ARR;   //自动重装载值
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器3
//	
//	//初始化TIM3 PWM模式	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
// 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
//	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
// 
//	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
//	
//	TIM_Cmd(TIM1, ENABLE);  //使能TIM3
//	

}  






void Set_PWM_VAL(u8 PWM_NUM,u32 VAL){
	////VAL 0-100 代表0%-100%
	u32 PWM_VAL=0;
	PWM_VAL=VAL*50;
	switch(PWM_NUM){
		case PWM1:
			TIM_SetCompare1(TIM1,PWM_VAL);	//修改比较值，修改占空比
			break;
		case PWM2:
			TIM_SetCompare2(TIM1,PWM_VAL);
			break;
		case PWM3:
			TIM_SetCompare3(TIM1,PWM_VAL);
			break;
		case PWM4:
			TIM_SetCompare4(TIM1,PWM_VAL);
			break;
		default:
			break;	
	}
	
	
	
}

