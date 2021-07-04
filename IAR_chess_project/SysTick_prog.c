#include"macros.h"
#include"std_types.h"
#include"SysTick_interface.h"
#include"SysTick_Private.h"



void SysTick_INIT(void)
{
  STRELOAD = SYSTICK_VALUE;
  STCURRENT = 0;
  STCTRL |= 0X03;
  
  
  
}


void SysTick_Fun(void)
{

  
}