#ifndef _INPUT_CAPTURE_H
#define _INPUT_CAPTURE_H

#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#define INPUT_CH1 	//PA6 TIM3
#define INPUT_CH2	//PA7	TIM3
#define INPUT_CH3	//PB0	TIM3
#define INPUT_CH4	//PB1 	TIM3

#define INPUT_CH5	//PA5	TIM2
#define INPUT_CH6	//PA1	TIM2
#define INPUT_CH7	//PA2	TIM2
#define INPUT_CH8	//PA3	TIM2

void TIM3_CAP_Init(void);
void TIM2_CAP_Init(void);
u32 TIM3_CH1_VAL(void);
u32 TIM3_CH2_VAL(void);
u32 TIM3_CH3_VAL(void);
u32 TIM3_CH4_VAL(void);

#endif
