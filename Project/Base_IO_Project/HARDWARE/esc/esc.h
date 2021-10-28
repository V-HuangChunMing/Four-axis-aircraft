#ifndef _ESC_H
#define _ESC_H

#include "sys.h"
#include "pwm.h"
//ESC1 PE9
//ESC2 PE11
//ESC3 PE13
//ESC4 PE14
void Set_ESC_1(u32 VAL);
void Set_ESC_2(u32 VAL);
void Set_ESC_3(u32 VAL);
void Set_ESC_4(u32 VAL);
//VAL 0-100 ´ú±í0%-100%

void Unlock_ESC_1(void);
void Unlock_ESC_2(void);
void Unlock_ESC_3(void);
void Unlock_ESC_4(void);
void Unlock_ESC_All(void);


#endif 
