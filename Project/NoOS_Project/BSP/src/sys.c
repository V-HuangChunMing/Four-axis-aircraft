#include "sys.h"

#define System_Clock 16

void Sys_Init(void){

	Sys_Clock_Init();


}

//���Ч�ʿ���ʹ�û�࣬��Ҫ�ȴ�ʱ���ȶ�
void Sys_Clock_Init(void){
	
	RCC->CR|=BIT16;//ʹ��HSE
	
	RCC->PLLCFGR=BIT_CLEAR;//׼������MainPLL
	RCC->PLLCFGR|=BIT2|BIT9|BIT11|BIT13|BIT22;//����ʱ�ӷ�Ƶϵ��Ϊ4����Ƶ��Ϊ8����PLL��Ƶ����Ϊ2
	
	RCC->CFGR
}
