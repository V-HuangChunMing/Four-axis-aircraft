#include "esc.h"

/*最高油门行程为脉宽为2ms，最低油门行程为1ms，占空比跟频率有关*/
/*过程为
1. 油门打到最高点
2. 电调接上电池，等待两秒
3. 将油门推到最低 等待一秒

在125Hz下，占空比从5%缓慢增加到10%，整个过程大概耗时>2S即可。
*/

//此处代码为84Mhz，1Mhz 的频率
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


//解锁
void Unlock_ESC_All(void)
{
	Set_ESC_1(20);
	Set_ESC_2(20);
	Set_ESC_3(20);
	Set_ESC_4(20);	
}


//设定油门行程
void Set_Throttle_Pos(void)
{
	u32 i=10;
	//i为23时同时旋转
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
