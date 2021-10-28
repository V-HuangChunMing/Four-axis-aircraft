#include "iic.h"
/*
SDA1 PD12
SCL1 PD13

SDA2 PD14
SCL2 PD15
*/
void IIC_1_Init(void)                //初始化IIC的IO口
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
		IIC_1_SCL=1;
		IIC_1_SDA=1;  //SCL和SDA都拉高
}
void IIC_1_Start(void)			//发送IIC开始信号
{
	SDA_1_OUT();     //sda线输出
	IIC_1_SDA=1;	  	  
	IIC_1_SCL=1;
	delay_us(4);
 	IIC_1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_1_SCL=0;//钳住I2C总线，准备发送或接收数据
}
void IIC_1_Stop(void)	  			//发送IIC停止信号
{
	SDA_1_OUT();//sda线输出
	IIC_1_SCL=0;
	IIC_1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_1_SCL=1; 
	IIC_1_SDA=1;//发送I2C总线结束信号
	delay_us(4);


}

void IIC_1_Send_Byte(u8 txd)			//IIC发送一个字节
{
    u8 t;   
	SDA_1_OUT(); 	    
    IIC_1_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_1_SDA=(txd&0x80)>>7;//先传高位再传地位
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_1_SCL=1;
		delay_us(2); 
		IIC_1_SCL=0;	
		delay_us(2);
    }



}


u8 IIC_1_Read_Byte(unsigned char ack)//IIC读取一个字节
{
	unsigned char i,receive=0;
	SDA_1_IN();//SDA设置为输入
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
        IIC_1_NAck();//发送nACK
    else
        IIC_1_Ack(); //发送ACK   
    return receive;	
}



u8 IIC_1_Wait_Ack(void) 				//IIC等待ACK信号
{
	u8 ucErrTime=0;
	SDA_1_IN();      //SDA设置为输入  
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
	IIC_1_SCL=0;//时钟输出0 	   
	return 0;  	
	
}
void IIC_1_Ack(void)//IIC发送ACK信号
{
	IIC_1_SCL=0;
	SDA_1_OUT();
	IIC_1_SDA=0;
	delay_us(2);
	IIC_1_SCL=1;
	delay_us(2);
	IIC_1_SCL=0;
}
void IIC_1_NAck(void)				//IIC不发送ACK信号
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
void IIC_2_Init(void)                //初始化IIC的IO口
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOB时钟

	//GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
		IIC_2_SCL=1;
		IIC_2_SDA=1;  //SCL和SDA都拉高
}
void IIC_2_Start(void)			//发送IIC开始信号
{
	SDA_1_OUT();     //sda线输出
	IIC_2_SDA=1;	  	  
	IIC_2_SCL=1;
	delay_us(4);
 	IIC_2_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_2_SCL=0;//钳住I2C总线，准备发送或接收数据
}
void IIC_2_Stop(void)	  			//发送IIC停止信号
{
	SDA_1_OUT();//sda线输出
	IIC_2_SCL=0;
	IIC_2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_2_SCL=1; 
	IIC_2_SDA=1;//发送I2C总线结束信号
	delay_us(4);


}

void IIC_2_Send_Byte(u8 txd)			//IIC发送一个字节
{
    u8 t;   
	SDA_1_OUT(); 	    
    IIC_2_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_2_SDA=(txd&0x80)>>7;//先传高位再传地位
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_2_SCL=1;
		delay_us(2); 
		IIC_2_SCL=0;	
		delay_us(2);
    }



}


u8 IIC_2_Read_Byte(unsigned char ack)//IIC读取一个字节
{
	unsigned char i,receive=0;
	SDA_1_IN();//SDA设置为输入
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
        IIC_2_NAck();//发送nACK
    else
        IIC_2_Ack(); //发送ACK   
    return receive;	
}



u8 IIC_2_Wait_Ack(void) 				//IIC等待ACK信号
{
	u8 ucErrTime=0;
	SDA_1_IN();      //SDA设置为输入  
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
	IIC_2_SCL=0;//时钟输出0 	   
	return 0;  	
	
}
void IIC_2_Ack(void)//IIC发送ACK信号
{
	IIC_2_SCL=0;
	SDA_1_OUT();
	IIC_2_SDA=0;
	delay_us(2);
	IIC_2_SCL=1;
	delay_us(2);
	IIC_2_SCL=0;
}


void IIC_2_NAck(void)				//IIC不发送ACK信号
{
	IIC_2_SCL=0;
	SDA_1_OUT();
	IIC_2_SDA=1;
	delay_us(2);
	IIC_2_SCL=1;
	delay_us(2);
	IIC_2_SCL=0;
}





