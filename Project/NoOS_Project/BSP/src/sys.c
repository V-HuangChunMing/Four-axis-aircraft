#include "sys.h"

#define System_Clock 16

void Sys_Init(void){

	Sys_Clock_Init();


}

//提高效率可以使用汇编，需要等待时钟稳定
void Sys_Clock_Init(void){
	
	RCC->CR|=BIT16;//使能HSE
	
	RCC->PLLCFGR=BIT_CLEAR;//准备配置MainPLL
	RCC->PLLCFGR|=BIT2|BIT9|BIT11|BIT13|BIT22;//输入时钟分频系数为4、倍频数为8、主PLL分频倍数为2
	
	RCC->CFGR
}
