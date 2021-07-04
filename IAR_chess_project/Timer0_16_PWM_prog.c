
#include"macros.h"
#include"std_types.h"
#include"Timer0_16_PWM_interface.h"
#include"Timer0_16_PWM_private.h"
#include"Timer0_16_PWM_config.h"

void Timer0_16_init(void)
{
  
  
RCGCTIMER|=   0x00000001;  // clock to timer 0  16
RCGCGPIO  |=  0x00000020; //clock port F
GPIOAFSEL |=  0x00000001 ; //alternate function
GPIOPCTL  &= ~0x00000F00 ; 
GPIOPCTL  |=  0x00000007 ; // assign pin F0 function as timer 0 CM
GPIOAMSEL &= ~0x00000001; //disable analoge
GPIODEN   |=  0x00000001; // enable digital

GPTMCTL = 0; 
GPTMCTL |= 0x00000040; //inverted output
  
GPTMCFG = 0x00000004;      
     
GPTMTAMR = 0x0000080A ;        
  
GPTMTAILR =  0x00186A00 ; // 20 ms , 50 hz at 80 MHZ 

GPTMTAMATCHR = 0; // no match

GPTMCTL |= 0x00000001; 

}

void Set_T0_16_Match(u32 match)// 0 -> 1 600 000
{
  
  
GPTMTAMATCHR = match;

}