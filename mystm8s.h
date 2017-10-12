#ifndef __MYSTM8S207__H
#define __MYSTM8S207__H

#include "stm8s.h"

void CLK_CONFIG_16MHZ_HSE(void);
void Delay_Using_Timer4_Init(void);
void Delay_ISR(void);
void Delay_ms(uint32_t time);

#endif

