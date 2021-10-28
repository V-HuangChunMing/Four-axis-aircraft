#include "iic.h"
/*
SDA1 PD12
SCL1 PD13

SDA2 PD14
SCL2 PD15
*/
void IIC_1_Init(void)                //��ʼ��IIC��IO��
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
		IIC_1_SCL=1;
		IIC_1_SDA=1;  //SCL��SDA������
}
void IIC_1_Start(void)			//����IIC��ʼ�ź�
{
	SDA_1_OUT();     //sda�����
	IIC_1_SDA=1;	  	  
	IIC_1_SCL=1;
	delay_us(4);
 	IIC_1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_1_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
void IIC_1_Stop(void)	  			//����IICֹͣ�ź�
{
	SDA_1_OUT();//sda�����
	IIC_1_SCL=0;
	IIC_1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_1_SCL=1; 
	IIC_1_SDA=1;//����I2C���߽����ź�
	delay_us(4);


}

void IIC_1_Send_Byte(u8 txd)			//IIC����һ���ֽ�
{
    u8 t;   
	SDA_1_OUT(); 	    
    IIC_1_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_1_SDA=(txd&0x80)>>7;//�ȴ���λ�ٴ���λ
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_1_SCL=1;
		delay_us(2); 
		IIC_1_SCL=0;	
		delay_us(2);
    }



}


u8 IIC_1_Read_Byte(unsigned char ack)//IIC��ȡһ���ֽ�
{
	unsigned char i,receive=0;
	SDA_1_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_1_SCL=0; 
        delay_us(2);
		IIC_1_SCL=1;
        receive<<=1;
        if(READ_1_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_1_NAck();//����nACK
    else
        IIC_1_Ack(); //����ACK   
    return receive;	
}



u8 IIC_1_Wait_Ack(void) 				//IIC�ȴ�ACK�ź�
{
	u8 ucErrTime=0;
	SDA_1_IN();      //SDA����Ϊ����  
	IIC_1_SDA=1;delay_us(1);	   
	IIC_1_SCL=1;delay_us(1);	 
	while(READ_1_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_1_Stop();
			return 1;
		}
	}
	IIC_1_SCL=0;//ʱ�����0 	   
	return 0;  	
	
}
void IIC_1_Ack(void)//IIC����ACK�ź�
{
	IIC_1_SCL=0;
	SDA_1_OUT();
	IIC_1_SDA=0;
	delay_us(2);
	IIC_1_SCL=1;
	delay_us(2);
	IIC_1_SCL=0;
}
void IIC_1_NAck(void)				//IIC������ACK�ź�
{
	IIC_1_SCL=0;
	SDA_1_OUT();
	IIC_1_SDA=1;
	delay_us(2);
	IIC_1_SCL=1;
	delay_us(2);
	IIC_1_SCL=0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void IIC_2_Init(void)                //��ʼ��IIC��IO��
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��

	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
		IIC_2_SCL=1;
		IIC_2_SDA=1;  //SCL��SDA������
}
void IIC_2_Start(void)			//����IIC��ʼ�ź�
{
	SDA_1_OUT();     //sda�����
	IIC_2_SDA=1;	  	  
	IIC_2_SCL=1;
	delay_us(4);
 	IIC_2_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_2_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
void IIC_2_Stop(void)	  			//����IICֹͣ�ź�
{
	SDA_1_OUT();//sda�����
	IIC_2_SCL=0;
	IIC_2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_2_SCL=1; 
	IIC_2_SDA=1;//����I2C���߽����ź�
	delay_us(4);


}

void IIC_2_Send_Byte(u8 txd)			//IIC����һ���ֽ�
{
    u8 t;   
	SDA_1_OUT(); 	    
    IIC_2_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_2_SDA=(txd&0x80)>>7;//�ȴ���λ�ٴ���λ
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_2_SCL=1;
		delay_us(2); 
		IIC_2_SCL=0;	
		delay_us(2);
    }



}


u8 IIC_2_Read_Byte(unsigned char ack)//IIC��ȡһ���ֽ�
{
	unsigned char i,receive=0;
	SDA_1_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_2_SCL=0; 
        delay_us(2);
		IIC_2_SCL=1;
        receive<<=1;
        if(READ_1_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_2_NAck();//����nACK
    else
        IIC_2_Ack(); //����ACK   
    return receive;	
}



u8 IIC_2_Wait_Ack(void) 				//IIC�ȴ�ACK�ź�
{
	u8 ucErrTime=0;
	SDA_1_IN();      //SDA����Ϊ����  
	IIC_2_SDA=1;delay_us(1);	   
	IIC_2_SCL=1;delay_us(1);	 
	while(READ_1_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_2_Stop();
			return 1;
		}
	}
	IIC_2_SCL=0;//ʱ�����0 	   
	return 0;  	
	
}
void IIC_2_Ack(void)//IIC����ACK�ź�
{
	IIC_2_SCL=0;
	SDA_1_OUT();
	IIC_2_SDA=0;
	delay_us(2);
	IIC_2_SCL=1;
	delay_us(2);
	IIC_2_SCL=0;
}


void IIC_2_NAck(void)				//IIC������ACK�ź�
{
	IIC_2_SCL=0;
	SDA_1_OUT();
	IIC_2_SDA=1;
	delay_us(2);
	IIC_2_SCL=1;
	delay_us(2);
	IIC_2_SCL=0;
}




