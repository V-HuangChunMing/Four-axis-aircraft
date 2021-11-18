#include "input_capture.h"


#define PSC 86
#define ARR 499


//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数


//PA6
//PA7
//PB0
//PB7

void TIM3_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //GPIOA0---对应K1按键
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6|GPIO_PinSource7,GPIO_AF_TIM3); //PA0复用位定时器5
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0|GPIO_PinSource1,GPIO_AF_TIM3); //PA0复用位定时器5
	
	TIM_TimeBaseStructure.TIM_Prescaler=PSC;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=ARR;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_2|TIM_Channel_3|TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
		
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5

 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}



//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到低电平了.
//[5:0]:捕获高电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)

u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM3CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u8  TIM3CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM3CH2_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u8  TIM3CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM3CH3_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u8  TIM3CH4_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM3CH4_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)


void TIM3_IROHandler(void){
	//CH1
	//溢出中断处理部分
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)						//若bit7为0还未成功捕获//如果成功捕获就跳过处理，等待数据的读取	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检测是否发生中断
		 
		{	    											//若发生中断
			if(TIM3CH1_CAPTURE_STA&0X40)					//若bit6为1已经捕获到高电 0x40=0100 0000
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)		//若高电平太长溢出
				{
					TIM3CH1_CAPTURE_STA|=0X80;				//bit7赋值为1标记成功捕获了一次  val表示为最大值0xffff
					TIM3CH1_CAPTURE_VAL=0XFFFF;             
				}else TIM3CH1_CAPTURE_STA++;                 //否则就sta+1
			}	 
		}
	//捕获中断处理
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)			//捕获1发生捕获事件
			{	
				if(TIM3CH1_CAPTURE_STA&0X40)					//若bit6为0表示上一次捕获到上升沿这次捕获到一个下降沿 		
				{	  			
					TIM3CH1_CAPTURE_STA|=0X80;					//标记成功捕获到一次  上升沿
					TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);  //CC1P=0 设置为上升沿捕获
				}
				else  													//还未开始,第一次捕获上升沿
				{
					TIM3CH1_CAPTURE_STA=0;								//清空
					TIM3CH1_CAPTURE_VAL=0;
					TIM_SetCounter(TIM3,0);
					TIM3CH1_CAPTURE_STA|=0X40;							//标记捕获到了上升沿
					TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);	//设置为下降沿捕获
				}		    
			}			     	    					   
	}
 
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); 			//清除中断标志位
 
	
	//CH2
	if((TIM3CH2_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM3CH2_CAPTURE_STA&0X40)
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM3CH2_CAPTURE_STA|=0X80;
					TIM3CH2_CAPTURE_VAL=0XFFFF;
				}else TIM3CH2_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
		{	
			if(TIM3CH2_CAPTURE_STA&0X40)			
			{	  			
				TIM3CH2_CAPTURE_STA|=0X80;		
				TIM3CH2_CAPTURE_VAL=TIM_GetCapture2(TIM3);
		   		TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); 
			}else  								
			{
				TIM3CH2_CAPTURE_STA=0;			
				TIM3CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM3,0);
				TIM3CH2_CAPTURE_STA|=0X40;		
		   		TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		
			}		    
		}			     	    					   
 	}
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2|TIM_IT_Update); 
	
	//CH3
	if((TIM3CH3_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM3CH3_CAPTURE_STA&0X40)
			{
				if((TIM3CH3_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM3CH3_CAPTURE_STA|=0X80;
					TIM3CH3_CAPTURE_VAL=0XFFFF;
				}else TIM3CH3_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
		{	
			if(TIM3CH3_CAPTURE_STA&0X40)		
			{	  			
				TIM3CH3_CAPTURE_STA|=0X80;		
				TIM3CH3_CAPTURE_VAL=TIM_GetCapture3(TIM3);
		   		TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); 
			}else  								
			{
				TIM3CH3_CAPTURE_STA=0;			
				TIM3CH3_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM3,0);
				TIM3CH3_CAPTURE_STA|=0X40;		
		   		TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update); 
	
	//CH4
		if((TIM3CH4_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM3CH4_CAPTURE_STA&0X40)
			{
				if((TIM3CH4_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM3CH4_CAPTURE_STA|=0X80;
					TIM3CH4_CAPTURE_VAL=0XFFFF;
				}else TIM3CH4_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
		{	
			if(TIM3CH4_CAPTURE_STA&0X40)			
			{	  			
				TIM3CH4_CAPTURE_STA|=0X80;		
				TIM3CH4_CAPTURE_VAL=TIM_GetCapture4(TIM3);
		   		TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								
			{
				TIM3CH4_CAPTURE_STA=0;			
				TIM3CH4_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM5,0);
				TIM3CH4_CAPTURE_STA|=0X40;		
		   		TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4|TIM_IT_Update); 
	
}



//PA5
//PA1
//PA2
//PA3

void TIM2_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5; //GPIOA0---对应K1按键
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1|GPIO_PinSource2|GPIO_PinSource3|GPIO_PinSource5,GPIO_AF_TIM2); //PA0复用位定时器5
		
	TIM_TimeBaseStructure.TIM_Prescaler=PSC;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=ARR;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_2|TIM_Channel_3|TIM_Channel_4; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
		
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
	TIM_Cmd(TIM2,ENABLE ); 	//使能定时器5

 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}



//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到低电平了.
//[5:0]:捕获高电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)

u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u8  TIM2CH2_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH2_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u8  TIM2CH3_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH3_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u8  TIM2CH4_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH4_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)


void TIM2_IROHandler(void){
	//CH1
	//溢出中断处理部分
 	if((TIM2CH1_CAPTURE_STA&0X80)==0)						//若bit7为0还未成功捕获//如果成功捕获就跳过处理，等待数据的读取	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检测是否发生中断
		 
		{	    											//若发生中断
			if(TIM2CH1_CAPTURE_STA&0X40)					//若bit6为1已经捕获到高电 0x40=0100 0000
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)		//若高电平太长溢出
				{
					TIM2CH1_CAPTURE_STA|=0X80;				//bit7赋值为1标记成功捕获了一次  val表示为最大值0xffff
					TIM2CH1_CAPTURE_VAL=0XFFFF;             
				}else TIM2CH1_CAPTURE_STA++;                 //否则就sta+1
			}	 
		}
	//捕获中断处理
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)			//捕获1发生捕获事件
			{	
				if(TIM2CH1_CAPTURE_STA&0X40)					//若bit6为0表示上一次捕获到上升沿这次捕获到一个下降沿 		
				{	  			
					TIM2CH1_CAPTURE_STA|=0X80;					//标记成功捕获到一次  上升沿
					TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
					TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);  //CC1P=0 设置为上升沿捕获
				}
				else  													//还未开始,第一次捕获上升沿
				{
					TIM2CH1_CAPTURE_STA=0;								//清空
					TIM2CH1_CAPTURE_VAL=0;
					TIM_SetCounter(TIM5,0);
					TIM2CH1_CAPTURE_STA|=0X40;							//标记捕获到了上升沿
					TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);	//设置为下降沿捕获
				}		    
			}			     	    					   
	}
 
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); 			//清除中断标志位
 
	
	//CH2
	if((TIM2CH2_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH2_CAPTURE_STA&0X40)
			{
				if((TIM2CH2_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM2CH2_CAPTURE_STA|=0X80;
					TIM2CH2_CAPTURE_VAL=0XFFFF;
				}else TIM2CH2_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
		{	
			if(TIM2CH2_CAPTURE_STA&0X40)			
			{	  			
				TIM2CH2_CAPTURE_STA|=0X80;		
				TIM2CH2_CAPTURE_VAL=TIM_GetCapture2(TIM2);
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
			}else  								
			{
				TIM2CH2_CAPTURE_STA=0;			
				TIM2CH2_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH2_CAPTURE_STA|=0X40;		
		   		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		
			}		    
		}			     	    					   
 	}
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); 
	
	//CH3
	if((TIM2CH3_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH3_CAPTURE_STA&0X40)
			{
				if((TIM2CH3_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM2CH3_CAPTURE_STA|=0X80;
					TIM2CH3_CAPTURE_VAL=0XFFFF;
				}else TIM2CH3_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
		{	
			if(TIM2CH3_CAPTURE_STA&0X40)		
			{	  			
				TIM2CH3_CAPTURE_STA|=0X80;		
				TIM2CH3_CAPTURE_VAL=TIM_GetCapture3(TIM2);
		   		TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
			}else  								
			{
				TIM2CH3_CAPTURE_STA=0;			
				TIM2CH3_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH3_CAPTURE_STA|=0X40;		
		   		TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);		
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3|TIM_IT_Update); 
	
	//CH4
		if((TIM2CH4_CAPTURE_STA&0X80)==0)
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		 
		{	    
			if(TIM2CH4_CAPTURE_STA&0X40)
			{
				if((TIM2CH4_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM2CH4_CAPTURE_STA|=0X80;
					TIM2CH4_CAPTURE_VAL=0XFFFF;
				}else TIM2CH4_CAPTURE_STA++;
			}	 
		}
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
		{	
			if(TIM2CH4_CAPTURE_STA&0X40)			
			{	  			
				TIM2CH4_CAPTURE_STA|=0X80;		
				TIM2CH4_CAPTURE_VAL=TIM_GetCapture4(TIM2);
		   		TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else  								
			{
				TIM2CH4_CAPTURE_STA=0;			
				TIM2CH4_CAPTURE_VAL=0;
	 			TIM_SetCounter(TIM2,0);
				TIM2CH4_CAPTURE_STA|=0X40;		
		   		TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);		
			}		    
		}			     	    					   
 	}
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4|TIM_IT_Update); 
	
}




/*
temp3=TIM5CH4_CAPTURE_STA&0X3F;
temp3*=65536;
temp3+=TIM5CH4_CAPTURE_VAL;
//printf("CH4:%d us\r\n",temp3);此处计算出的是高电平时长
TIM5CH4_CAPTURE_STA=0;
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态		    				
//u32	TIM3CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
////定时器5中断服务程序	 
//void TIM5_IRQHandler(void)
//{ 		    

// 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
//	{
//		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
//		{	     
//			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
//			{
//				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//				{
//					TIM3CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
//					TIM3CH1_CAPTURE_VAL=0XFFFFFFFF;
//				}else TIM3CH1_CAPTURE_STA++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
//		{	
//			if(TIM3CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
//			{	  			
//				TIM3CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
//			  TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//获取当前的捕获值.
//	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
//			}else  								//还未开始,第一次捕获上升沿
//			{
//				TIM3CH1_CAPTURE_STA=0;			//清空
//				TIM3CH1_CAPTURE_VAL=0;
//				TIM3CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
//				TIM_Cmd(TIM5,DISABLE ); 	//关闭定时器5
//	 			TIM_SetCounter(TIM5,0);
//	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
//				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
//			}		    
//		}			     	    					   
// 	}
//	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
//}
