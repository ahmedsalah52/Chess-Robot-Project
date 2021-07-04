#include"macros.h"
#include"std_types.h"
#include"PLL_private.h" 
#include"PLL_interface.h" 




void PLL_INIT(void)
{
  RCC2 |=  0x80000000; // rcc2 on  //SET 31 , 30 ,28 TO 23  -> 02  ,22 -> 0, SET 11 , 
  
  RCC2 |=  0x00000800; // set the bybass2
   
  RCC &= ~0x000007c0;   //CLEAR 6 - 10 
  RCC |=  0x00000540 ; //16 MHZ
  
  RCC2 &= ~0x00000070; //clear 4 - 6 bits to select main occ

  RCC2 &= ~0x00002000; // enable pll by clearing pwrden2
    
  RCC2 |= 0x40000000; // divider 400
  
  RCC2 &= ~0x1fc00000; // CLEAR CLOCK DEVISOR
  
  RCC2 |= 0x00800000; // SET CLOCK DEVISOR FOR 80 MHZ
  
  RCC2 &=  ~0x00000800; // enable PLL by clearing the bybass2
    

}
