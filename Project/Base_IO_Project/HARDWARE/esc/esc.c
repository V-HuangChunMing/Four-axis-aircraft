#include "esc.h"

/*��������г�Ϊ����Ϊ2ms����������г�Ϊ1ms��ռ�ձȸ�Ƶ���й�*/
/*����Ϊ
1. ���Ŵ���ߵ�
2. ������ϵ�أ��ȴ�����
3. �������Ƶ���� �ȴ�һ��

��125Hz�£�ռ�ձȴ�5%�������ӵ�10%���������̴�ź�ʱ>2S���ɡ�
*/

//�˴�����Ϊ84Mhz��1Mhz ��Ƶ��
void Set_ESC_1(u32 VAL)
{
	Set_PWM_VAL(PWM1,VAL);
}
	
void Set_ESC_2(u32 VAL)
{
	Set_PWM_VAL(PWM2,VAL);
}
void Set_ESC_3(u32 VAL)
{
	Set_PWM_VAL(PWM3,VAL);
}
void Set_ESC_4(u32 VAL)
{
	Set_PWM_VAL(PWM4,VAL);
}


void Unlock_ESC_1(void)
{
	Set_ESC_1(60);
}
void Unlock_ESC_2(void)
{
	Set_ESC_2(60);
}
void Unlock_ESC_3(void)
{
	Set_ESC_3(60);
}
void Unlock_ESC_4(void)
{
	Set_ESC_4(60);	
}


//����
void Unlock_ESC_All(void)
{
	Set_ESC_1(20);
	Set_ESC_2(20);
	Set_ESC_3(20);
	Set_ESC_4(20);	
}


//�趨�����г�
void Set_Throttle_Pos(void)
{
	u32 i=10;
	//iΪ23ʱͬʱ��ת
	for(i=48;i>20;i--){
		Set_ESC_1(i);
		delay_ms(10);
		Set_ESC_2(i);
		delay_ms(10);
		Set_ESC_3(i);
		delay_ms(10);
		Set_ESC_4(i);
		
	}
	Set_ESC_1(20);
	Set_ESC_2(20);
	Set_ESC_3(20);
	Set_ESC_4(20);
	
	
	

}
