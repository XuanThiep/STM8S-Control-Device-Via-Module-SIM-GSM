#include "mystm8s.h"
#include "stm8s_clk.h"

volatile uint32_t time_keeper=0;

void CLK_CONFIG_16MHZ_HSE(void)
{
  CLK_DeInit();
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}



void Delay_Using_Timer4_Init(void)
{
   TIM4_TimeBaseInit(TIM4_PRESCALER_128,124);//1ms if fMaster=16Mhz
   TIM4_ClearFlag(TIM4_FLAG_UPDATE);
   TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);
    
   enableInterrupts();
   TIM4_Cmd(ENABLE);
}

/* Implement it in INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23) */
void Delay_ISR(void)
{
  if(TIM4_GetITStatus(TIM4_IT_UPDATE)==SET)
  {
    if(time_keeper!=0)
    {
      time_keeper--;
    }
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
  }
}


void Delay_ms(uint32_t time)
{
  time_keeper=time;
  while(time_keeper);
}
