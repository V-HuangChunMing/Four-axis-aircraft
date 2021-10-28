#ifndef _IIC_H
#define _IIC_H


#include	"sys.h"
#include    "delay.h"

/*
使用软件模拟IIC
SDA1 PD12
SCL1 PD13

SDA2 PD14
SCL2 PD15

*/
//IO方向设置
#define SDA_1_IN() 		{GPIOD->MODER&=~(3<<(12*2));GPIOD->MODER|=0<<12*2;}	//PB9输入模式
#define SDA_1_OUT()		{GPIOD->MODER&=~(3<<(12*2));GPIOD->MODER|=1<<12*2;} //PB9输出模式
#define SCL_1_IN() 		{GPIOD->MODER&=~(3<<(13*2));GPIOD->MODER|=0<<13*2;}	//PB9输入模式
#define SCL_1_OUT()		{GPIOD->MODER&=~(3<<(13*2));GPIOD->MODER|=1<<13*2;} //PB9输出模式
#define SDA_2_IN()		{GPIOD->MODER&=~(3<<(14*2));GPIOD->MODER|=0<<14*2;}	//PB9输入模式
#define SDA_2_OUT()		{GPIOD->MODER&=~(3<<(14*2));GPIOD->MODER|=1<<14*2;} //PB9输出模式
#define SCL_2_IN() 		{GPIOD->MODER&=~(3<<(15*2));GPIOD->MODER|=0<<15*2;}	//PB9输入模式
#define SCL_2_OUT()		{GPIOD->MODER&=~(3<<(15*2));GPIOD->MODER|=1<<15*2;} //PB9输出模式
//IO操作地址
#define IIC_1_SCL	PDout(13)
#define IIC_1_SDA	PDout(12)
#define READ_1_SDA    PDin(12)
#define READ_1_SCL    PDin(13)

#define IIC_2_SCL	PDout(15)
#define IIC_2_SDA	PDout(14)
#define READ_2_SDA    PDin(14)
#define READ_2_SCL    PDin(15)
//IIC所有操作函数
//IIC1
void IIC_1_Init(void);                //初始化IIC的IO口				 
void IIC_1_Start(void);				//发送IIC开始信号
void IIC_1_Stop(void);	  			//发送IIC停止信号
void IIC_1_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_1_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_1_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_1_Ack(void);					//IIC发送ACK信号
void IIC_1_NAck(void);				//IIC不发送ACK信号

//void IIC_1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_1_Read_One_Byte(u8 daddr,u8 addr);	  



//IIC2
void IIC_2_Init(void);                //初始化IIC的IO口				 
void IIC_2_Start(void);				//发送IIC开始信号
void IIC_2_Stop(void);	  			//发送IIC停止信号
void IIC_2_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_2_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_2_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_2_Ack(void);					//IIC发送ACK信号
void IIC_2_NAck(void);				//IIC不发送ACK信号

//void IIC_2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_2_Read_One_Byte(u8 daddr,u8 addr);	



#endif 
