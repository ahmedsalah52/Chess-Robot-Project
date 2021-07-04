#include"PLL_interface.h" 
#include"macros.h"
#include"std_types.h"
#include"SysTick_interface.h"
#include"Timer0_PWM_interface.h"
#include"Timer1_PWM_interface.h"
#include"Timer0_16_PWM_interface.h"
#include"Timer5_PWM_interface.h"
#include"Robot_interface.h"
#include"GPIO_interface.h"
#include"chess_config.h"
#include"robot_config.h"



u16 x , y , z , h ,l;
u8 X,Y,Z;
u64 i;

int main()
{
 int  test=304000;
 
  
   Robot_INIT();
 
   GPIO_voidSetPinValue(Main_M_Port_Steps,Main_M_Pin_Steps,LOW);

   
   
 
   
   
   Start_Game();
  GPIO_voidSetPinValue(PORT_B,PIN_2,LOW);
 
   Set_Robot_Angles( Base_Default_angle ,Main_Default_angle , Arm_Default_angle) ;

   
   
   while(1)
    {//b2 e0 
   
         Check_Board();

   
   
 GPIO_voidSetPinValue(PORT_B,PIN_2,LOW);
    Set_Robot_Angles( Base_Default_angle ,Main_Default_angle , Arm_Default_angle) ;
  
        
        Robots_Turn();
 
   
    Set_Robot_Angles( 3000 ,Main_initial_angle , Arm_initial_angle) ;
    
    Set_Robot_Angles( Base_initial_angle ,Main_initial_angle , Arm_initial_angle) ;

    Delay_ms(100);
    
    GPIO_voidSetPinValue(PORT_B,PIN_2,HIGH);
 
 
       
    }
  
  return 0;
}

