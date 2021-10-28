#ifndef _IIC_H
#define _IIC_H


#include	"sys.h"
#include    "delay.h"

/*
ʹ�����ģ��IIC
SDA1 PD12
SCL1 PD13

SDA2 PD14
SCL2 PD15

*/
//IO��������
#define SDA_1_IN() 		{GPIOD->MODER&=~(3<<(12*2));GPIOD->MODER|=0<<12*2;}	//PB9����ģʽ
#define SDA_1_OUT()		{GPIOD->MODER&=~(3<<(12*2));GPIOD->MODER|=1<<12*2;} //PB9���ģʽ
#define SCL_1_IN() 		{GPIOD->MODER&=~(3<<(13*2));GPIOD->MODER|=0<<13*2;}	//PB9����ģʽ
#define SCL_1_OUT()		{GPIOD->MODER&=~(3<<(13*2));GPIOD->MODER|=1<<13*2;} //PB9���ģʽ
#define SDA_2_IN()		{GPIOD->MODER&=~(3<<(14*2));GPIOD->MODER|=0<<14*2;}	//PB9����ģʽ
#define SDA_2_OUT()		{GPIOD->MODER&=~(3<<(14*2));GPIOD->MODER|=1<<14*2;} //PB9���ģʽ
#define SCL_2_IN() 		{GPIOD->MODER&=~(3<<(15*2));GPIOD->MODER|=0<<15*2;}	//PB9����ģʽ
#define SCL_2_OUT()		{GPIOD->MODER&=~(3<<(15*2));GPIOD->MODER|=1<<15*2;} //PB9���ģʽ
//IO������ַ
#define IIC_1_SCL	PDout(13)
#define IIC_1_SDA	PDout(12)
#define READ_1_SDA    PDin(12)
#define READ_1_SCL    PDin(13)

#define IIC_2_SCL	PDout(15)
#define IIC_2_SDA	PDout(14)
#define READ_2_SDA    PDin(14)
#define READ_2_SCL    PDin(15)
//IIC���в�������
//IIC1
void IIC_1_Init(void);                //��ʼ��IIC��IO��				 
void IIC_1_Start(void);				//����IIC��ʼ�ź�
void IIC_1_Stop(void);	  			//����IICֹͣ�ź�
void IIC_1_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_1_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_1_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_1_Ack(void);					//IIC����ACK�ź�
void IIC_1_NAck(void);				//IIC������ACK�ź�

//void IIC_1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_1_Read_One_Byte(u8 daddr,u8 addr);	  



//IIC2
void IIC_2_Init(void);                //��ʼ��IIC��IO��				 
void IIC_2_Start(void);				//����IIC��ʼ�ź�
void IIC_2_Stop(void);	  			//����IICֹͣ�ź�
void IIC_2_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_2_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_2_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_2_Ack(void);					//IIC����ACK�ź�
void IIC_2_NAck(void);				//IIC������ACK�ź�

//void IIC_2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_2_Read_One_Byte(u8 daddr,u8 addr);	



#endif 
