#ifndef _PWM_H
#define _PWM_H

#include "sys.h"
#define PWM1	1//PE9
#define PWM2	2//PE11
#define PWM3	3//PE13
#define PWM4	4//PE14

#define PSC 83//ʱ��Ԥ��Ƶ��
#define ARR 499//�Զ���װֵ


void TIM1_PWM_Init(void);

void Set_PWM_VAL(u8 PWM_NUM,u32 PWM_VAL);


#endif
