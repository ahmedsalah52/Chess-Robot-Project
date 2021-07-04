
#include"macros.h"
#include"std_types.h"
#include"Timer5_PWM_interface.h"
#include"Timer5_PWM_private.h"
#include"Timer5_PWM_config.h"

void Timer5_32_init(void)
{
  
  
RCGCWTIMER |=  0x00000020; // clock to timer 5 
RCGCGPIO   |=  0x00000008; //clock port D
GPIOAFSEL  |=  0x00000040; //alternate function
GPIOPCTL   &= ~0x07000000; 
GPIOPCTL   |=  0x07000000; // assign pin D7 function as timer 5 CM
GPIOAMSEL  &= ~0x00000040; //disable analoge
GPIODEN    |=  0x00000040; // enable digital

GPTMCTL = 0; 
GPTMCTL |= 0x00000040; //inverted output
  
GPTMCFG = 0x00000004;      
     
GPTMTAMR = 0x0000080A ;        
  
GPTMTAILR =  0x00186A00 ; // 20 ms , 50 hz at 80 MHZ 

GPTMTAMATCHR = 0; // no match

GPTMCTL |= 0x00000001; 

}

void Set_T5_Match(u32 match)// 0 -> 1 600 000
{
  
  
GPTMTAMATCHR = match;

}