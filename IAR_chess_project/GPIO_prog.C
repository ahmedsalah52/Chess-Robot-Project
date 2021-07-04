#include"macros.h"
#include"std_types.h"
#include"GPIO_interface.h"
#include"GPIO_private.h"



void GPIO_voidSetPinDir(u8 PORT , u8 PIN , u8 Dir) //OUTPUT 1
{
  SYSCTL_RCGC2_R |= 1<<PORT ;  // 1)  clock

  switch(PORT)
  {
    
  case 0:
  GPIO_PORTA_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTA_CR_R |= (1<<PIN);      // allow changes to PIN (GPIOAFSEL, GPIOPUR, GPIOPDR,or GPIODEN       
  GPIO_PORTA_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTA_DIR_R |= (Dir<<PIN) ;  // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTA_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTA_DEN_R |= (1<<PIN);   // 7) enable digital pins PF4-PF0         

    break;

case 1:
  GPIO_PORTB_LOCK_R = 0x4C4F434B;     
  GPIO_PORTB_CR_R |= (1<<PIN);           
  GPIO_PORTB_AMSEL_R = 0x00;       
  GPIO_PORTB_PCTL_R = 0x00000000;  
  GPIO_PORTB_DIR_R |= (Dir<<PIN) ;    
  GPIO_PORTB_AFSEL_R = 0x00;       
  GPIO_PORTB_DEN_R |= (1<<PIN);          
    
    break;

case 2:
  GPIO_PORTC_LOCK_R = 0x4C4F434B;  
  GPIO_PORTC_CR_R |= (1<<PIN);           
  GPIO_PORTC_AMSEL_R = 0x00;        
  GPIO_PORTC_PCTL_R = 0x00000000;   
  GPIO_PORTC_DIR_R |= (Dir<<PIN) ;    
  GPIO_PORTC_AFSEL_R = 0x00;       
  GPIO_PORTC_DEN_R |= (1<<PIN);  
  
    
    break;

case 3:
  GPIO_PORTD_LOCK_R = 0x4C4F434B;    
  GPIO_PORTD_CR_R |= (1<<PIN);             
  GPIO_PORTD_AMSEL_R = 0x00;       
  GPIO_PORTD_PCTL_R = 0x00000000;    
  GPIO_PORTD_DIR_R |= (Dir<<PIN) ;    
  GPIO_PORTD_AFSEL_R = 0x00;        
  GPIO_PORTD_DEN_R |= (1<<PIN);            
    
    break;

case 4:
  GPIO_PORTE_LOCK_R = 0x4C4F434B;    
  GPIO_PORTE_CR_R |= (1<<PIN);            
  GPIO_PORTE_AMSEL_R = 0x00;        
  GPIO_PORTE_PCTL_R = 0x00000000;    
  GPIO_PORTE_DIR_R |= (Dir<<PIN) ;    
  GPIO_PORTE_AFSEL_R = 0x00;        
  GPIO_PORTE_DEN_R |= (1<<PIN);            
 
    break;

case 5:
  
  GPIO_PORTF_LOCK_R = 0x4C4F434B;    
  GPIO_PORTF_CR_R |= (1<<PIN);              
  GPIO_PORTF_AMSEL_R = 0x00;        
  GPIO_PORTF_PCTL_R = 0x00000000;   
  GPIO_PORTF_DIR_R |= (Dir<<PIN) ;     
  GPIO_PORTF_AFSEL_R = 0x00;        
  GPIO_PORTF_DEN_R |= (1<<PIN);           

    break;

  }


}

void GPIO_voidSetInputPin_PR(u8 PIN , u8 Pull_up) // enable pull up 1
{//All GPIO signals are 5-V tolerant when configured as inputs
//except for PD4, PD5, PB0 and PB1
  
  GPIO_PORTF_PUR_R |= (Pull_up<<PIN);          // enable pullup resistors on PIN       
}

void GPIO_voidSetPinValue(u8 PORT , u8 Pin , u8 Value)
{
  if(Value == 1)
  {
    SET_BIT(GPIO_PORTF_DATA_R , Pin);  
  }
  else if (Value ==0)
  {
    CLEAR_BIT(GPIO_PORTF_DATA_R , Pin);
  }

}


u8 GPIO_u8GetPinValue(u8 PORT , u8 Pin )
{
  switch (PORT)
  {
case 0:
  
  return (GPIO_PORTA_DATA_R>>Pin)&1;

case 1:
  
  return (GPIO_PORTB_DATA_R>>Pin)&1;

case 2:
  
  return (GPIO_PORTC_DATA_R>>Pin)&1;

case 3:
  
  return (GPIO_PORTD_DATA_R>>Pin)&1;

case 4:
  
  return (GPIO_PORTE_DATA_R>>Pin)&1;

case 5:
  
  return (GPIO_PORTF_DATA_R>>Pin)&1;
  }

}