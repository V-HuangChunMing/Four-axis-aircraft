#include "esc.h"


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

/*��������г�Ϊ����Ϊ2ms����������г�Ϊ1ms��ռ�ձȸ�Ƶ���й�*/
/*����Ϊ
1. ���Ŵ���ߵ�
2. ������ϵ�أ��ȴ�����
3. �������Ƶ���� �ȴ�һ��

��125Hz�£�ռ�ձȴ�5%�������ӵ�10%���������̴�ź�ʱ>2S���ɡ�
*/

//�˴�����Ϊ84Mhz��1Mhz ��Ƶ��
void Unlock_ESC_1(void)
{
	
}
void Unlock_ESC_2(void)
{
}
void Unlock_ESC_3(void)
{
}
void Unlock_ESC_4(void)
{
}
void Unlock_ESC_All(void)
{
	u32 i=10;
	
	for(i=10;i<20;i++){
		Set_ESC_1(i);
		Set_ESC_1(i);
		Set_ESC_1(i);
		Set_ESC_1(i);
		
		delay_ms(100);
		delay_ms(100);
	}
	
}
