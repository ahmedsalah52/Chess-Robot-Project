
#include"macros.h"
#include"std_types.h"
#include"Timer0_PWM_interface.h"
#include"Timer0_PWM_private.h"
#include"Timer0_PWM_config.h"
#include"tm4c_cmsis.h"
#include"GPIO_interface.h"

extern u8 x;

void Timer0_32_init(void)
{
  
  
RCGCWTIMER |=0x00000001;  // clock to timer 0 
RCGCGPIO |= 0x00000004; //clock port c
GPIOAFSEL |= 0x00000010 ; //alternate function
GPIOPCTL &= ~0x000f0000 ; 
GPIOPCTL |= 0x00070000 ; // assign pin c4 function as timer 0 CM
GPIOAMSEL &= ~0x00000010; //disable analoge
GPIODEN |= 0x00000010; // enable digital

GPTMCTL = 0; 
GPTMCTL |= 0x00000000; // not inverted output , bit 2:3 positive edge interrupt mode , so the result will be positive edge interrupt
  
GPTMCFG = 0x00000004;      
     
GPTMTAMR = 0x00000A0A ;

  
GPTMTAILR =  320000 ; //0x00186A00 ; // 20 ms , 50 hz at 80 MHZ 

GPTMTAMATCHR = 0; // no match

//REG6INTEN  |= 0x40000000;  //ENABLE interrupt bit timer 0   94 order  bit 30

//GPTMIMR |= 0x04;  // enable timeout interrupt A

GPTMCTL |= 0x00000001; 

}

void Set_T0_Match(u32 match)// 0 -> 1 600 000
{
  
  
GPTMTAMATCHR = match;

}

void WideTimer0A_IRQHandler(void)
{
   x^=1;
  
  GPIO_voidSetPinValue(PORT_A , PIN_7 , x);

}