
#include"macros.h"
#include"std_types.h"
#include"Timer1_PWM_interface.h"
#include"Timer1_PWM_private.h"
#include"Timer1_PWM_config.h"

void Timer1_32_init(void)
{
  
  
RCGCWTIMER|=  0x00000002;  // clock to timer 1 
RCGCGPIO  |=  0x00000004; //clock port c
GPIOAFSEL |=  0x00000040 ; //alternate function
GPIOPCTL  &= ~0x0f000000 ; 
GPIOPCTL  |=  0x07000000 ; // assign pin C6 function as timer 1 CM
GPIOAMSEL &= ~0x00000040; //disable analoge
GPIODEN   |=  0x00000040; // enable digital

GPTMCTL = 0; 
GPTMCTL |= 0x00000040; //inverted output
  
GPTMCFG = 0x00000004;      
     
GPTMTAMR = 0x0000080A ;        
  
GPTMTAILR =  0x00186A00 ; // 20 ms , 50 hz at 80 MHZ 

GPTMTAMATCHR = 0; // no match

GPTMCTL |= 0x00000001; 

}

void Set_T1_Match(u32 match)// 0 -> 1 600 000
{
  
  
GPTMTAMATCHR = match;

}