#include "input_capture.h"


#define PSC 86
#define ARR 499


//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��


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

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //GPIOA0---��ӦK1����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6|GPIO_PinSource7,GPIO_AF_TIM3); //PA0����λ��ʱ��5
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0|GPIO_PinSource1,GPIO_AF_TIM3); //PA0����λ��ʱ��5
	
	TIM_TimeBaseStructure.TIM_Prescaler=PSC;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=ARR;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_2|TIM_Channel_3|TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
		
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��5

 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}



//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����ߵ�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)

u8  TIM3CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM3CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u8  TIM3CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM3CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u8  TIM3CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM3CH3_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u8  TIM3CH4_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM3CH4_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)


void TIM3_IROHandler(void){
	//CH1
	//����жϴ�����
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)						//��bit7Ϊ0��δ�ɹ�����//����ɹ���������������ȴ����ݵĶ�ȡ	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //����Ƿ����ж�
		 
		{	    											//�������ж�
			if(TIM3CH1_CAPTURE_STA&0X40)					//��bit6Ϊ1�Ѿ����񵽸ߵ� 0x40=0100 0000
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)		//���ߵ�ƽ̫�����
				{
					TIM3CH1_CAPTURE_STA|=0X80;				//bit7��ֵΪ1��ǳɹ�������һ��  val��ʾΪ���ֵ0xffff
					TIM3CH1_CAPTURE_VAL=0XFFFF;             
				}else TIM3CH1_CAPTURE_STA++;                 //�����sta+1
			}	 
		}
	//�����жϴ���
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)			//����1���������¼�
			{	
				if(TIM3CH1_CAPTURE_STA&0X40)					//��bit6Ϊ0��ʾ��һ�β�����������β���һ���½��� 		
				{	  			
					TIM3CH1_CAPTURE_STA|=0X80;					//��ǳɹ�����һ��  ������
					TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM3);
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);  //CC1P=0 ����Ϊ�����ز���
				}
				else  													//��δ��ʼ,��һ�β���������
				{
					TIM3CH1_CAPTURE_STA=0;								//���
					TIM3CH1_CAPTURE_VAL=0;
					TIM_SetCounter(TIM3,0);
					TIM3CH1_CAPTURE_STA|=0X40;							//��ǲ�����������
					TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);	//����Ϊ�½��ز���
				}		    
			}			     	    					   
	}
 
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); 			//����жϱ�־λ
 
	
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
		   		TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
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

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5; //GPIOA0---��ӦK1����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1|GPIO_PinSource2|GPIO_PinSource3|GPIO_PinSource5,GPIO_AF_TIM2); //PA0����λ��ʱ��5
		
	TIM_TimeBaseStructure.TIM_Prescaler=PSC;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=ARR;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1|TIM_Channel_2|TIM_Channel_3|TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
		
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��5

 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}



//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����ߵ�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)

u8  TIM2CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u8  TIM2CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM2CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u8  TIM2CH3_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM2CH3_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u8  TIM2CH4_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM2CH4_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)


void TIM2_IROHandler(void){
	//CH1
	//����жϴ�����
 	if((TIM2CH1_CAPTURE_STA&0X80)==0)						//��bit7Ϊ0��δ�ɹ�����//����ɹ���������������ȴ����ݵĶ�ȡ	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //����Ƿ����ж�
		 
		{	    											//�������ж�
			if(TIM2CH1_CAPTURE_STA&0X40)					//��bit6Ϊ1�Ѿ����񵽸ߵ� 0x40=0100 0000
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)		//���ߵ�ƽ̫�����
				{
					TIM2CH1_CAPTURE_STA|=0X80;				//bit7��ֵΪ1��ǳɹ�������һ��  val��ʾΪ���ֵ0xffff
					TIM2CH1_CAPTURE_VAL=0XFFFF;             
				}else TIM2CH1_CAPTURE_STA++;                 //�����sta+1
			}	 
		}
	//�����жϴ���
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)			//����1���������¼�
			{	
				if(TIM2CH1_CAPTURE_STA&0X40)					//��bit6Ϊ0��ʾ��һ�β�����������β���һ���½��� 		
				{	  			
					TIM2CH1_CAPTURE_STA|=0X80;					//��ǳɹ�����һ��  ������
					TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM2);
					TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);  //CC1P=0 ����Ϊ�����ز���
				}
				else  													//��δ��ʼ,��һ�β���������
				{
					TIM2CH1_CAPTURE_STA=0;								//���
					TIM2CH1_CAPTURE_VAL=0;
					TIM_SetCounter(TIM5,0);
					TIM2CH1_CAPTURE_STA|=0X40;							//��ǲ�����������
					TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);	//����Ϊ�½��ز���
				}		    
			}			     	    					   
	}
 
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); 			//����жϱ�־λ
 
	
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
		   		TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
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
//printf("CH4:%d us\r\n",temp3);�˴���������Ǹߵ�ƽʱ��
TIM5CH4_CAPTURE_STA=0;
*/



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//u8  TIM3CH1_CAPTURE_STA=0;	//���벶��״̬		    				
//u32	TIM3CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
////��ʱ��5�жϷ������	 
//void TIM5_IRQHandler(void)
//{ 		    

// 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//	{
//		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
//		{	     
//			if(TIM3CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					TIM3CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
//					TIM3CH1_CAPTURE_VAL=0XFFFFFFFF;
//				}else TIM3CH1_CAPTURE_STA++;
//			}	 
//		}
//		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
//		{	
//			if(TIM3CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
//			{	  			
//				TIM3CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
//			  TIM3CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
//	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}else  								//��δ��ʼ,��һ�β���������
//			{
//				TIM3CH1_CAPTURE_STA=0;			//���
//				TIM3CH1_CAPTURE_VAL=0;
//				TIM3CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
//				TIM_Cmd(TIM5,DISABLE ); 	//�رն�ʱ��5
//	 			TIM_SetCounter(TIM5,0);
//	 			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
//			}		    
//		}			     	    					   
// 	}
//	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
//}
