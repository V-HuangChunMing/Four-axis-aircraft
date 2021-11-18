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

/*最高油门行程为脉宽为2ms，最低油门行程为1ms，占空比跟频率有关*/
/*过程为
1. 油门打到最高点
2. 电调接上电池，等待两秒
3. 将油门推到最低 等待一秒

在125Hz下，占空比从5%缓慢增加到10%，整个过程大概耗时>2S即可。
*/

//此处代码为84Mhz，1Mhz 的频率
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
