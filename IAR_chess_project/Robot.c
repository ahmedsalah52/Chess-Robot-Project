#include"PLL_interface.h" 
#include"macros.h"
#include"std_types.h"
#include"Timer0_PWM_interface.h"
#include"Timer1_PWM_interface.h"
#include"Timer0_16_PWM_interface.h"
#include"Timer5_PWM_interface.h"
#include"GPIO_interface.h"
#include"robot_config.h"
#include"Robot_interface.h"
#include<math.h>
#include"chess_config.h"


extern u8 chess_board[8][8];
u8 King_INIT_POS;
u8 board_inputs[8]={0};
u8 First_move;
u16 Curr_B , Curr_M , Curr_A;
u8 Next_Base_Sign , Last_Base_Sign;

typedef struct Positions
{
  u8 Base_Angle;
  u8 Main_Angle;
  u8 Arm_Angle;
  
}Position_Angles;

void Robot_INIT(void)
{
  King_INIT_POS= 0x37;
  First_move=1;
  Last_Base_Sign= 1;
  Next_Base_Sign =1;
  Timer0_32_init();
  //Set_T0_Match(304000);
  GPIO_voidSetPinDir(PORT_B,PIN_5,INPUT);
  GPIO_voidSetInputPin_PDR(PORT_B,PIN_5,HIGH);
  
  GPIO_voidSetPinDir(PORT_D,PIN_0,OUTPUT);
  GPIO_voidSetPinDir(PORT_D,PIN_1,OUTPUT);
  GPIO_voidSetPinDir(PORT_D,PIN_2,OUTPUT);
  GPIO_voidSetPinDir(PORT_D,PIN_3,OUTPUT);
  GPIO_voidSetPinDir(PORT_E,PIN_1,OUTPUT);
  GPIO_voidSetPinDir(PORT_E,PIN_2,OUTPUT);
  GPIO_voidSetPinDir(Buzzer_PORT ,Buzzer_PIN , OUTPUT);

  GPIO_voidSetPinDir(Base_M_Port_Steps,Base_M_Pin_Steps,OUTPUT);
  GPIO_voidSetPinDir(Main_M_Port_Steps,Main_M_Pin_Steps,OUTPUT);
  GPIO_voidSetPinDir(Arm_M_Port_Steps,Arm_M_Pin_Steps,OUTPUT);
  GPIO_voidSetPinDir(Base_M_Port_Dir,Base_M_Pin_Dir,OUTPUT);
  GPIO_voidSetPinDir(Main_M_Port_Dir,Main_M_Pin_Dir,OUTPUT);
  GPIO_voidSetPinDir(Arm_M_Port_Dir,Arm_M_Pin_Dir,OUTPUT);

  GPIO_voidSetPinDir(Gripper_PORT,Gripper_PIN,OUTPUT);

Curr_B = Base_initial_angle;
Curr_M = Main_initial_angle;  
Curr_A = Arm_initial_angle;
  
  
GPIO_voidSetPinDir(PORT_B,PIN_2,OUTPUT);
GPIO_voidSetPinDir(PORT_E,PIN_0,OUTPUT);
  
}

void Robots_Move(u8 Init_Pos , u8 Next_Pos)
{
  if(Check_Cell(Column(Next_Pos),Row(Next_Pos))==1) // next pos is NOT free
  {
    Make_Robot_Move(Next_Pos , Garbage_Pos);    
    

    Make_Robot_Move(Init_Pos , Next_Pos);    
  }
  else
  {   
     Make_Robot_Move(Init_Pos , Next_Pos); 
  }

Set_Robot_Angles( 3000 ,Main_Default_angle , Arm_Default_angle) ;

}

void Make_Robot_Move(u8 Init_Pos , u8 Next_Pos)
{

Go_Position(Row(Init_Pos),Column(Init_Pos) , 1 );

Open_Gripper();
Delay_ms(50);

Go_Position(Row(Init_Pos),Column(Init_Pos) , 0 );
Delay_ms(50);

Close_Gripper();
Delay_ms(50);

Go_Position(Row(Init_Pos),Column(Init_Pos) , 1 );
Delay_ms(50);

//Release_Gripper();

Go_Position(Row(Next_Pos),Column(Next_Pos) , 1 );
Delay_ms(50);

Go_Position(Row(Next_Pos),Column(Next_Pos) , 0 );
Delay_ms(50);

Open_Gripper();
Delay_ms(50);

Go_Position(Row(Next_Pos),Column(Next_Pos) , 1 );
Close_Gripper();
Delay_ms(50);

Release_Gripper();


}
void Go_Position(u8 Row,u8 Column , u8 Hight )
{ 
  
  f64 X_Position , Y_Position , Z_Position ;
  f64 Theta_1 ,Theta_2,Theta_3 ;
  f64 fi1 ,fi2;
  f64  r1 , range ;
  
  
  if(Column <= 3)
  {
    Column = 3 - Column;
    Next_Base_Sign = 0;
   
  }
  else
  {
    Column = Column - 4;
    Next_Base_Sign = 1;
  
  }
  

  Column ++;
  Column --;
  if((Row == 0) && (Column <= 1)) //first row
  {
  X_Position = (134.38) + (((f64)Row * 23.75) );
  Y_Position  = ((23.75)/2) + (((f64)Column *23.75) ) ;
  Z_Position = (16+ Z_offset )+((f64)Hight *35) ;
    
  }
  else
  {
  X_Position = (134.38) + (((f64)Row * 23.75) );
  Y_Position  = ((23.75)/2) + (((f64)Column *23.75) ) ;
  Z_Position = (8 + Z_offset )+((f64)Hight *55);
  }
  
  
  
  Theta_1 = (atan((Y_Position)/(X_Position)));

  range = sqrt(pow(X_Position,2)+pow(Y_Position,2)) + range_offset;// - (((f64)(10)) *(Hight));
  
  
  r1 = sqrt( pow(range ,2) + pow((Z_Position - A_1),2));

  
  Theta_3 = acos(((pow(A_2,2))+(pow(A_3,2))-(pow(r1,2)))/(2*A_2*A_3)); 
 

  fi1 = atan((Z_Position - A_1)/(range));
  
  fi2 = asin((sin(Theta_3) * A_3)/(r1));
  
  Theta_2 =  fi1 + fi2 ;
    
  Theta_1 = (Theta_1 * 180)/3.14;
  Theta_2 = (Theta_2 * 180)/3.14;
  Theta_3 = (Theta_3 * 180)/3.14;

  Set_Robot_Angles((u16)(100 * Theta_1),(u16)(100* Theta_2), (u16)(100*Theta_3));

  
}
void Set_Robot_Angles(u16 Base_Angle , u16 Main_Angle , u16 Arm_Angle )// *100 // base sign positive 1 , negative 0  
{

  u8 Times,B_Dir,M_Dir,A_Dir; 
  u16 Theta_Base , Theta_Main , Theta_Arm;
  u32 B_count , M_count , A_count;
  u32 B_done , M_done , A_done;
  
  B_done =0;
  M_done =0;
  A_done =0;
  B_Dir = 0;
  M_Dir = 1;
  A_Dir = 1;
 

      if(Next_Base_Sign == 0 && Last_Base_Sign == 1)
      {
         Theta_Base = Curr_B + Base_Angle;  
         B_Dir = 0; 
        Last_Base_Sign = 0;
      }
      else if(Next_Base_Sign == 1 && Last_Base_Sign == 0)
      {
        Theta_Base = Curr_B + Base_Angle;  
        B_Dir = 1; 
        Last_Base_Sign = 1;

      }
      else if((Next_Base_Sign == 0) && (Last_Base_Sign == 0))
      {
        // BASE degree
      if(Curr_B > Base_Angle)
      {
         Theta_Base = Curr_B - Base_Angle;  
         B_Dir = 1; 
      }
      else if(Curr_B < Base_Angle)
      {
        Theta_Base = Base_Angle - Curr_B ;  
        B_Dir = 0; 
      }
      else
      {     
        Theta_Base = 0; 
        B_Dir = 1; 
      }
      }
      else if((Next_Base_Sign == 1) && (Last_Base_Sign == 1))
      {
        // BASE degree
      if(Curr_B > Base_Angle)
      {
         Theta_Base = Curr_B - Base_Angle;  
         B_Dir = 0; 
      }
      else if(Curr_B < Base_Angle)
      {
        Theta_Base = Base_Angle - Curr_B ;  
        B_Dir = 1; 
      }
      else
      {     
        Theta_Base = 0; 
        B_Dir = 1; 
      }
      }
      
      //Main arm degree
       if(Curr_M > Main_Angle)
      {
         Theta_Main = Curr_M - Main_Angle;  
         M_Dir = 0; 
      }
      else if(Curr_M < Main_Angle)
      {
        Theta_Main = Main_Angle - Curr_M ;  
        M_Dir = 1; 
      }
      else
      {     
        Theta_Main = 0; 
        M_Dir = 1; 
      }
       
      //Arm degree
       if(Curr_A > Arm_Angle)
      {
         Theta_Arm = Curr_A - Arm_Angle;  
         A_Dir = 0; 
      }
      else if(Curr_A < Arm_Angle)
      {
        Theta_Arm = Arm_Angle - Curr_A ;  
        A_Dir = 1; 
      }
      else
      {     
        Theta_Arm = 0; 
        A_Dir = M_Dir; 
      }

        
   
      
   
  //gear ratio 49/21   110 / 21
 // test how to handle negative nums
  
 B_count = (((16*10*((u32)Theta_Base)*Base_Gear_No_Teeth*B_error_factor)/(Stepper_Gear_No_Teeth*18))/100) ;
 M_count = ((16*10*((u32)Theta_Main)*Main_Gear_No_Teeth*M_error_factor)/(Stepper_Gear_No_Teeth*18))/100;
 A_count = ((16*10*((u32)Theta_Arm)*Arm_Gear_No_Teeth*A_error_factor)/(Stepper_Gear_No_Teeth*18))/100;
 
 if(M_Dir == A_Dir)
 {
   A_count = M_count + A_count;
 }
 else
 {
   if( A_count < M_count)
   {
     A_count = M_count - A_count;
     A_Dir = 1 - A_Dir;
   }
   else
   {
      A_count = A_count - M_count ;
   }
}
 
 GPIO_voidSetPinValue(Base_M_Port_Dir,Base_M_Pin_Dir,B_Dir);
 GPIO_voidSetPinValue(Main_M_Port_Dir,Main_M_Pin_Dir,M_Dir);
 GPIO_voidSetPinValue(Arm_M_Port_Dir,Arm_M_Pin_Dir,A_Dir);

 if((M_count<700)&&(A_count <850))
 {
   B_count+=5;
   Times = 0;
 while(Times<divisor_degree)
 {
   while((B_done <(B_count/divisor_degree) )||(M_done < (M_count/divisor_degree))||(A_done < (A_count/divisor_degree)))
 {
 
   if(B_done<(B_count/divisor_degree))
   {
   GPIO_voidSetPinValue(Base_M_Port_Steps,Base_M_Pin_Steps,HIGH);
   B_done++;
   }
   

   if(M_done<(M_count/divisor_degree))
   { 
   GPIO_voidSetPinValue(Main_M_Port_Steps,Main_M_Pin_Steps,HIGH);
   M_done++;
   }
   
   
   if(A_done<(A_count/divisor_degree))
   {
   GPIO_voidSetPinValue(Arm_M_Port_Steps,Arm_M_Pin_Steps,HIGH);
   A_done++;
   }
   
   
   Delay_CC(100);  
   
   
   GPIO_voidSetPinValue(Base_M_Port_Steps,Base_M_Pin_Steps,LOW);
   
   GPIO_voidSetPinValue(Main_M_Port_Steps,Main_M_Pin_Steps,LOW);
   
   GPIO_voidSetPinValue(Arm_M_Port_Steps,Arm_M_Pin_Steps,LOW);
 
   Delay_CC(2000);  
     
  }
 
  B_done = 0;
  M_done = 0;
  A_done = 0;
 
  Times ++;
  
 }
 }
else
{
 while((B_done <B_count )||(M_done < M_count)||(A_done < A_count))
 {
 
   if(B_done<B_count)
   {
   GPIO_voidSetPinValue(Base_M_Port_Steps,Base_M_Pin_Steps,HIGH);
   B_done++;
   }
   

   if(M_done<M_count)
   { 
   GPIO_voidSetPinValue(Main_M_Port_Steps,Main_M_Pin_Steps,HIGH);
   M_done++;
   }
   
   
   if(A_done<A_count)
   {
   GPIO_voidSetPinValue(Arm_M_Port_Steps,Arm_M_Pin_Steps,HIGH);
   A_done++;
   }
   
   
   Delay_CC(100);  
   
   
   GPIO_voidSetPinValue(Base_M_Port_Steps,Base_M_Pin_Steps,LOW);
   
   GPIO_voidSetPinValue(Main_M_Port_Steps,Main_M_Pin_Steps,LOW);
   
   GPIO_voidSetPinValue(Arm_M_Port_Steps,Arm_M_Pin_Steps,LOW);
 
   Delay_CC(2000);  
  
    
  }

  B_done = 0;
  M_done = 0;
  A_done = 0;
 

} 
  
 
  
  Curr_B = Base_Angle;
  Curr_M = Main_Angle;
  Curr_A = Arm_Angle;
}
 
  
void Open_Gripper(void)
{
Set_T0_Match(301000); //301000
}

void Close_Gripper(void)
{
Set_T0_Match(294000);//290000
}

void Release_Gripper(void)
{
Set_T0_Match(0);
}

u8 Check_Cell(u8 column , u8 row)
{
  
   GPIO_voidSetPinValue(PORT_D,PIN_0,((column>>0)&1));
   GPIO_voidSetPinValue(PORT_D,PIN_1,((column>>1)&1));   
   GPIO_voidSetPinValue(PORT_D,PIN_2,((column>>2)&1));
                 
   GPIO_voidSetPinValue(PORT_D,PIN_3,((row>>0)&1));
   GPIO_voidSetPinValue(PORT_E,PIN_1,((row>>1)&1));
   GPIO_voidSetPinValue(PORT_E,PIN_2,((row>>2)&1));
                 
   if(GPIO_u8GetPinValue(PORT_B,PIN_5) == 0) 
       {
          return 1;           
       }
   else if(GPIO_u8GetPinValue(PORT_B,PIN_5) == 1)
       {
          return 0;
       }
 
}
void Start_Game(void)
{
  u8 row,column,ready;

   u8 First_Formation[8]={255,255,0,0,0,0,255,255};
  
ready=0;   
 while(First_move ==1)
{
 
  if(ready == 0)
  {
     Check_Board();

  }
  
  ready =1;
  
  
  for(column =0;column<=7;column++)
  {
     for(row=0;row<=7;row++)
     {
          if(( First_Formation[(row)]& (~(1<<(column))) )  ==( board_inputs[(row)]& (~(1<<(column))) ) )
              {
       
              }
              else
              {
                 ready =0;
            Wrong_Move();
              }
     }
  
  }

  if(ready == 1)
  {
    First_move=0;
  Right_Move();
  }

}
  
  
}

void Check_Board(void)
{
  
  
    u8 row,column,i,done ;
    u8 input[8]={0};  
    u8 Moves[5]={0xff,0xff,0xff,0xff,0xff};
    u8 Move_From , Move_To;
    
    Move_From=0;
    Move_To = 1;
    done =0;

    while(done ==0)
    {
      
    for(column=0;column<=7;column++)
    {//columns
  
          GPIO_voidSetPinValue(PORT_D,PIN_0,((column>>0)&1));
          GPIO_voidSetPinValue(PORT_D,PIN_1,((column>>1)&1));   
          GPIO_voidSetPinValue(PORT_D,PIN_2,((column>>2)&1));
          
          for(row=0;row<=7; row++)
               {
                     //rows from the bottom
          
                 GPIO_voidSetPinValue(PORT_D,PIN_3,((row>>0)&1));
                 GPIO_voidSetPinValue(PORT_E,PIN_1,((row>>1)&1));
                 GPIO_voidSetPinValue(PORT_E,PIN_2,((row>>2)&1));
             
              //the inputs are saved in a temprory array called 'input' then compared with the main array ' board_inputs'
                 
              if(First_move == 1) // if its the first move the inputs will be directly fill the board_inputs array
               {
                 if(GPIO_u8GetPinValue(PORT_B,PIN_5) == 0) 
                   {
                      board_inputs[(row)] |= 1<<(column); 
                   }
                 else if(GPIO_u8GetPinValue(PORT_B,PIN_5) == 1)
                   {
                      board_inputs[(row)] &= (~(1<<(column)));
                   }
               
                 if((row == 7) &&(column == 7))
                 {
                 done =1;                
                 }
               }
                else   //else its a normal move the inputs will fill the normal inputs array
                {
                 if(GPIO_u8GetPinValue(PORT_B,PIN_5) == 0)
                   {
                      input[(row)] |= 1<<(column); 
                   }
                 else if(GPIO_u8GetPinValue(PORT_B,PIN_5) == 1)
                   {
                      input[(row)] &= (~(1<<(column)));
                   }
                 //check the moves
                
                 
                 if( ( input[(row)]& (1<<(column)) ) != ( board_inputs[(row)]& (1<<(column)) ) )
                   { //their are two probabilites here if the input not equal the board input
                     //the first one input = 1 , so board input =0  a piece moved to this place 
                     //the second one input = 0 , so board = 1      a piece moved from this place 
                     if( (((input[row]>>column) &1) == 1) ) // the first probability a piece moved to this place
                     {
                       if(Moves[Move_To-2] !=((column<<4) + row)) //ensure that the move is a new move
                          {
                           Moves[Move_To]= (column<<4) + row; // 0b 0000      0000  
                                                          // 0b x(column) y(row)
                           Move_To+=2;                        //The move_to type are saved in odd order in the array while the move_from are in even
                          }
                     }                                  
                     else if(((input[row]>>column) &1) == 0)
                       // the second probability a piece moved from this place 
                       //The move_From type are saved in even order in the array while the move_to are in odd
                     {
                       if((Moves[Move_From-2] !=((column<<4) + row))&&(Moves[Move_From-4] !=((column<<4) + row)))
                          {
                    
                            Moves[Move_From] = (column<<4) + row;
                     
                            Move_From+=2;
                    
                          
                          }
                     }
                
                    }

                    //if one order of even (the replaced pieces )is now 1 so its a piece replaced and placed again (the killed piece)
                   //and it will be saved in odd order
                 if((( ((input[Row(Moves[0])]>>Column(Moves[0]))&1) == 1) | (((input[Row(Moves[2])]>>Column(Moves[2]))&1) == 1) )&(Moves[1]==0xff))
                       {
                         Moves[Move_To]= (column<<4) + row;  
                                                          
                           Move_To+=2; 
                         
                       }
           
                
                }
  
               }
 
    
    }

    
    
   if((Moves[4] == 0xff) &&  (Moves[3] == 0xff))
   {
     if((Moves[0] != 0xff)&&(Moves[1] != 0xff))
     {
       done = Check_Move(Moves); //while loop break condition
  
       if(done == 0)
       {
             Wrong_Move();
         for(i=0;i<5;i++)        //if there are alot of moves which is not legal the array will be cleared by oxff
          {
             Moves[i]= 0xff;   
          }
          Move_From=0;
          Move_To = 1;

       }
       else
       {
         Right_Move();
       }
     }
     
    }
    else 
    {  
        Wrong_Move();
       for(i=0;i<5;i++)        //if there are alot of moves which is not legal the array will be cleared by oxff
          {
             Moves[i]= 0xff;   
          }
       Move_From=0;
       Move_To = 1;
    }
  }

    
}

// this fun check if the move is legal or now , who moved and who is the killed and who is the killer
// if the move is legal , change it in the array (done by check_legal_move() function ) and return 1 if not return 0

u8 Check_Move(u8*Moves)
{
  u8 Piece_number;
  u8 Killer;
  u8 Killed;
  u8 Killer_Pos;
  u8 i;
  
  Killed =0;
  Killer =0;
  Killer_Pos = 0xff;
  if((Moves[0]!=0xff )&& (Moves[1]!=0xff ) && (Moves[2]==0xff ) ) //this case is a normal move from even order position to odd order position
  {
     
   Piece_number = Which_Piece( (Moves[0]&(0xf0))>>4 ,  Moves[0] & 0x0f ); // this function returns the piece number
    
    return Check_Legal_Move(Piece_number , Moves[0] , Moves[1] ); //from position 0 to position 1 
  }
  else if ((Moves[0]!=0xff )&& (Moves[1]!=0xff ) && (Moves[2]!=0xff ) ) // this case is a hunt
  {
    for(i=0;i<=2;i+=2)
    {
    Piece_number = Which_Piece( (Moves[i]&0xf0)>>4 , (Moves[i]&0x0f) ); // this function returns the piece number
    if((Piece_number%2)==1)  //if the piece number is odd (robot's piece) 
    {
         
     if( Moves[i] == Moves[1] )  // if it moved and another one took its place
     {
        Killed = Piece_number;   //the piece number will be saved to killed var
     }
    
    }
    else if(((Piece_number%2)==0)&&(Piece_number !=0)) //the piece num is even so its the opponent move 
    {
      Killer = Piece_number;       
      Killer_Pos = Moves[i];
    }
    }
  
    if(Killed !=0 && Killer!=0)  //if the killer and killed vars are  declared so its done
    {
      return Check_Legal_Move(Killer , Killer_Pos , Moves[1] ); // send the Killer num and the initial position and the killed position(Moves 1) 
    }                                                           // to see if the move is legal or not
    else 
    {
      return 0;
    }
  
  }

  
}

u8 Which_Piece(u8 column , u8 row ) // this function is used to determine the number of piece in the position x,y in the main chess_board array
{
  if((column <8) &&(column >=0)&&(row <8) &&(row >=0))
  {
  return chess_board[row][column];
  }
  else
  {
    return 0;
  }
 }
u8 Check_Legal_Move(u8 Piece_number , u8 Initial_POS, u8 Next_POS) // this function to return 1 if the move of this piece is legal and change it in the array
{
  
  switch (Piece_number)
  {
  case E_pawn:
   return E_Pawn(Initial_POS , Next_POS); 
   
  case E_queen:
   return E_Queen(Initial_POS,Next_POS);

  case E_bishop:
   return  E_Bishop(Initial_POS,Next_POS);  
  
  case E_rook:  
   return  E_Rook(Initial_POS,Next_POS);
  
  case E_knight:
    return   E_Knight(Initial_POS,Next_POS);

  case   E_king:
    return     E_King(Initial_POS,Next_POS);

  default:
    return 0;
      
  }
  
  
  }
  
  
  


u8 Make_Move(u8 Initial_Pos , u8 Next_POS)
{
  
  u8 Moved_Piece;
  Moved_Piece = Which_Piece( Column(Initial_Pos) , Row(Initial_Pos));
  // the next if statment is used to handle the chess board array changes in case of a pawn which reached row 0 (the end of the board) 
  // and all the pieces moves , after that we will have a copy of this if statment that undo the changes in case of any danger on the king
  
 if((Which_Piece( Column(Initial_Pos) , Row(Initial_Pos)) == E_pawn) && (Row(Next_POS)==0)) //if the pawn reached the end of the board
 {
  chess_board[Row(Next_POS)][Column(Next_POS)] =E_queen;
 }
 else
 {
 chess_board[Row(Next_POS)][Column(Next_POS)] = Which_Piece( Column(Initial_Pos) , Row(Initial_Pos)); //put the value of the moved piece
// in the next position  
 } 
 
 
if((Which_Piece( Column(Next_POS) , Row(Next_POS)) == E_king))
 {
   King_INIT_POS = Next_POS; 
 }

 chess_board[Row(Initial_Pos)][Column(Initial_Pos)] = 0; //clear the value from previous position
 
 
 if(CheckMate(Column(King_INIT_POS),Row(King_INIT_POS)) == 0) // if that move is a danger on the king 
 {
   //undo all the changes
    if((Moved_Piece == E_pawn) && (Row(Next_POS)==0)) //if the pawn reached the end of the board 
      // in case that the king is in danger 
    {
       chess_board[Row(Initial_Pos)][Column(Initial_Pos)] =E_pawn;
    
    }
    else
    {
       chess_board[Row(Initial_Pos)][Column(Initial_Pos)] = Which_Piece( Column(Next_POS) , Row(Next_POS)); 

       
       if(Which_Piece( Column(Initial_Pos) , Row(Initial_Pos)) == E_king)
       {
         King_INIT_POS = Initial_Pos; 
       }

    }     
    
    chess_board[Row(Next_POS)][Column(Next_POS)] = 0; //clear the value from Next position
    
    return 0; //refuse the move 
 
}
 
 
  
board_inputs[Row(Next_POS)] |= 1<<(Column(Next_POS)); 

board_inputs[Row(Initial_Pos)] &= (~(1<<(Column(Initial_Pos))));



 


return 1;
}


u8 Row(u8 Position)
{
  return Position&0x0f ;
}


u8 Column(u8 Position)
{
  return (Position&0xf0)>>4 ; 
}


void Wrong_Move(void)
{
  GPIO_voidSetPinValue(Buzzer_PORT ,Buzzer_PIN , HIGH);
  
}
void Right_Move(void)
{
  GPIO_voidSetPinValue(Buzzer_PORT ,Buzzer_PIN , LOW);

}


u8 E_Pawn( u8 Initial_POS, u8 Next_POS)
{
  
  if( (((Row(Initial_POS))-(Row(Next_POS))) == 2) && (((Column(Next_POS))==(Column(Initial_POS)))) && (((Row(Initial_POS))) == 6 ))  //first move so it can move 2 steps
      {
        if( Which_Piece( Column(Next_POS) , (Row(Next_POS))) == 0 )
        {
          return Make_Move(Initial_POS, Next_POS);
        }
        else
        {
          return 0;
        }
      }
    else if( (((Row(Initial_POS))-(Row(Next_POS))) == 1) && (((Column(Next_POS))==(Column(Initial_POS)))) ) //normal move 1 step 
      {
        if( Which_Piece( Column(Next_POS) , (Row(Next_POS))) == 0 )
        {
         return Make_Move(Initial_POS, Next_POS);
        }
        else
        {
          return 0;
        }

      }
    else if( (((Row(Initial_POS))-(Row(Next_POS))) == 1) && (  (((Column(Next_POS)) - (Column(Initial_POS)))==1) | (((Column(Initial_POS)) - (Column(Next_POS)))==1)    ) ) //hunting move  
      {
        
        if( Which_Piece( Column(Next_POS) , (Row(Next_POS))) > 0 )
        {
          return Make_Move(Initial_POS, Next_POS);
        }
        else
        {
          return 0;
        }

    
      }
    else 
      {
        return 0;
      }
      
  
}

u8 E_Queen( u8 Initial_POS, u8 Next_POS)
{
    u8 Row_offset ,Column_offset  , Right , Forward, row , column , Straight ;
    Row_offset = 0;
   Column_offset = 0;

    Straight = 0;
     Right = 2;
     Forward = 2;
     
     if(Row(Initial_POS)>Row(Next_POS))
    {
      Row_offset = Row(Initial_POS)-Row(Next_POS);
      Forward = 1;

    }
    else if(Row(Initial_POS)<Row(Next_POS))
    {
      Row_offset = Row(Next_POS)-Row(Initial_POS);
      Forward = 0;
    }
    else
    {
        Straight =1;
      
    }
    
    if(Column(Initial_POS)>Column(Next_POS))
    {
      Column_offset = Column(Initial_POS)-Column(Next_POS);
      Right = 1;
    }
    else if(Column(Initial_POS)<Column(Next_POS))
    {
      Column_offset = Column(Next_POS)-Column(Initial_POS);
      Right = 0;
    }
    else
    {
      Straight =1;
    
    }
    
    if((Straight==0) &&(Row_offset != Column_offset))
    {
      return 0;  // wrong move
    }
   
    
    row = Row(Initial_POS);
    column = Column(Initial_POS);
    
while(1)
  {
     if(Right == 1)
     {
       column --;
     }
     else if (Right == 0)
     {
       column ++;
     }
     if(Forward == 1)
     {
        row -- ;
     }
     else if(Forward == 0)
     {
       row++;
     }   
 
     /*
     if(Which_Piece(column ,row ) > 0)
     {
       if((Which_Piece(column ,row ) == Which_Piece(Column(Next_POS) ,Row(Next_POS) )) &&( (Which_Piece(column ,row )%2) ==1) )
         //if the last position is taken by an odd number it will be hunted
       {
         return Make_Move(Initial_POS, Next_POS);
       }
       else
       {
         return 0;
       }
     }
     else if((column ==Column(Next_POS)) &&( row == Row(Next_POS) )) 
       // reached the position with no obstacles
     {
        return Make_Move(Initial_POS, Next_POS);
     }
     else
     {
    
           return 0;
  
     }
       */
     
      if(Which_Piece(column ,row ) > 0)
     {
       if((Which_Piece(column ,row ) == Which_Piece(Column(Next_POS) ,Row(Next_POS) )) &&( (Which_Piece(column ,row )%2) ==1) )
         //if the last position is taken by an odd number it will be hunted
       {
         return Make_Move(Initial_POS, Next_POS);
       }
       else
       {
         return 0;
       }
     }
     else if((column ==Column(Next_POS)) &&( row == Row(Next_POS) )) 
       // reached the position with no obstacles
     {
        return Make_Move(Initial_POS, Next_POS);
     }
     else
     {
    
     
     }
     
    
}
    
        
}

u8 E_Bishop( u8 Initial_POS, u8 Next_POS)
{
    u8 Row_offset ,Column_offset  , Right , Forward, row , column  ;
     
    Row_offset = 0;
   Column_offset = 0;

     Right = 2;
     Forward = 2;
     
     if(Row(Initial_POS)>Row(Next_POS))
    {
      Row_offset = Row(Initial_POS)-Row(Next_POS);
      Forward = 1;

    }
    else if(Row(Initial_POS)<Row(Next_POS))
    {
      Row_offset = Row(Next_POS)-Row(Initial_POS);
      Forward = 0;
    }
    else
    {
      return 0;
    }
    
    if(Column(Initial_POS)>Column(Next_POS))
    {
      Column_offset = Column(Initial_POS)-Column(Next_POS);
      Right = 1;
    }
    else if(Column(Initial_POS)<Column(Next_POS))
    {
      Column_offset = Column(Next_POS)-Column(Initial_POS);
      Right = 0;
    }
    else
    {
      return 0;
    }
    
    if(Row_offset != Column_offset)
    {
      return 0;  // wrong move
    }
   
    
    row = Row(Initial_POS);
    column = Column(Initial_POS);
    
while(1)
  {
     if(Right == 1)
     {
       column --;
     }
     else if (Right == 0)
     {
       column ++;
     }
     if(Forward == 1)
     {
        row -- ;
     }
     else if(Forward == 0)
     {
       row++;
     }   
    
     if(Which_Piece(column ,row ) > 0)
     {
       if((Which_Piece(column ,row ) == Which_Piece(Column(Next_POS) ,Row(Next_POS) )) &&( (Which_Piece(column ,row )%2) ==1) )
         //if the last position is taken by an odd number it will be hunted
       {
         return Make_Move(Initial_POS, Next_POS);
       }
       else
       {
         return 0;
       }
     }
     else if((column ==Column(Next_POS)) &&( row == Row(Next_POS) )) 
       // reached the position with no obstacles
     {
        return Make_Move(Initial_POS, Next_POS);
     }
     else
     {
     
     }
       
    
  
}
    
        
}

u8 E_Rook( u8 Initial_POS, u8 Next_POS)
{
    u8  Right , Forward, row , column , Straight ;
  

    Straight = 0;
     Right = 2;
     Forward = 2;
     
     if(Row(Initial_POS)>Row(Next_POS))
    {
      Forward = 1;

    }
    else if(Row(Initial_POS)<Row(Next_POS))
    {
      Forward = 0;
    }
    else
    {
        Straight =1;
      
    }
    
    if(Column(Initial_POS)>Column(Next_POS))
    {
      Right = 1;
    }
    else if(Column(Initial_POS)<Column(Next_POS))
    {
      Right = 0;
    }
    else
    {
      Straight =1;
    
    }
    
    if((Straight==0))
    {
      return 0;  // wrong move
    }
   
    
    row = Row(Initial_POS);
    column = Column(Initial_POS);
    
while(1)
  {
     if(Right == 1)
     {
       column --;
     }
     else if (Right == 0)
     {
       column ++;
     }
     if(Forward == 1)
     {
        row -- ;
     }
     else if(Forward == 0)
     {
       row++;
     }   
    
     if(Which_Piece(column ,row ) > 0)
     {
       if((Which_Piece(column ,row ) == Which_Piece(Column(Next_POS) ,Row(Next_POS) )) &&( (Which_Piece(column ,row )%2) ==1) )
         //if the last position is taken by an odd number it will be hunted
       {
         return Make_Move(Initial_POS, Next_POS);
       }
       else
       {
         return 0;
       }
     }
     else if((column ==Column(Next_POS)) &&( row == Row(Next_POS) )) 
       // reached the position with no obstacles
     {
        return Make_Move(Initial_POS, Next_POS);
     }
     else
     {
    
     }
       
    
  
}
    


}


u8 E_Knight( u8 Initial_POS, u8 Next_POS)
{
  
   u8 Row_offset ,Column_offset  , Straight ;
     Straight = 0;
  Row_offset = 0;
   Column_offset = 0;
   
     if(Row(Initial_POS)>Row(Next_POS))
    {
      Row_offset = Row(Initial_POS)-Row(Next_POS);
   
    }
    else if(Row(Initial_POS)<Row(Next_POS))
    {
      Row_offset = Row(Next_POS)-Row(Initial_POS);
    }
    else
    {
        Straight =1;
      
    }
    
    if(Column(Initial_POS)>Column(Next_POS))
    {
      Column_offset = Column(Initial_POS)-Column(Next_POS);
    }
    else if(Column(Initial_POS)<Column(Next_POS))
    {
      Column_offset = Column(Next_POS)-Column(Initial_POS);
    }
    else
    {
      Straight =1;
    
    }
    
    if((Straight==1))
    {
      return 0;  // wrong move
    }
   
    
    //if the move is according to the knight move 
    if(((Column_offset == 1)&&(Row_offset == 2)) | ((Column_offset == 2)&&(Row_offset == 1)))
    {
       // if the next position is empty or taken by an odd piece (robots piece)
       if((Which_Piece(Column(Next_POS) ,Row(Next_POS) ) == 0) | ((Which_Piece(Column(Next_POS) ,Row(Next_POS)) %2 )==1)  ) //THE 
       {
         Make_Move(Initial_POS,Next_POS);
       }
       else
       {
         return 0;
       }
    }
    else
    {
      return 0;
    }
    
    
   
}




u8 E_King( u8 Initial_POS, u8 Next_POS)
{
   u8 Row_offset ,Column_offset  ,   Straight ;
   Row_offset = 0;
   Column_offset = 0;
   Straight = 0;
    
   if(Row(Initial_POS)>Row(Next_POS))
    {
      Row_offset = Row(Initial_POS)-Row(Next_POS);
     
    }
    else if(Row(Initial_POS)<Row(Next_POS))
    {
      Row_offset = Row(Next_POS)-Row(Initial_POS);
       }
    else
    {
        Straight =1;
      
    }
    
    if(Column(Initial_POS)>Column(Next_POS))
    {
      Column_offset = Column(Initial_POS)-Column(Next_POS);
   
    }
    else if(Column(Initial_POS)<Column(Next_POS))
    {
      Column_offset = Column(Next_POS)-Column(Initial_POS);
    }
    else
    {
      Straight =1;
    
    }
    
    if((Straight==0) &&(Row_offset != Column_offset))
    {
      return 0;  // wrong move
    }
   
    
    
 if(((Column_offset <=1)&&(Row_offset<=1)&&((Which_Piece( Column(Next_POS), Row(Next_POS) )==0)|((Which_Piece(Column(Next_POS), Row(Next_POS))%2)==1)) ))
   // if the move is only one step which is legal and the next position is free or opponent's piece
 { 
   
   if(CheckMate( Column(Next_POS) ,Row(Next_POS) ) == 0)
   {
     return 0;
   }
   else
   {
     return Make_Move(Initial_POS,Next_POS);
   }
 
 }
  else
 {
   return 0 ;
 }
 
 }


u8 CheckMate(u8 column , u8 row) //this function is called to check if their is a check mate in this place or not ,
//it returns 1 if safe  and 0 if their is a checkmate
{
  
  u8  inc ;
     
     inc =0;
   s8 i,j;
   u8 Surr[9]={0};  

  
  for(i = ((s8)row)-1 ; i >=0 ; i -- ) //first probability searching in the same column forward
   {
     if((Which_Piece(column,i) >0)&&(Which_Piece(column,i)!= E_king))
     {
       Surr[inc]= (column<<4)+i;
       inc ++;
       break;
     }
   }
    
   
   for(i = (s8)row+1; i <=7 ; i ++ ) // second probability searching in the same column backward
   {
     if((Which_Piece(column,i) >0)&&(Which_Piece(column,i)!= E_king))
     {
       Surr[inc]= (column<<4)+i;
       inc++;
       break;
     }
   }
  
   for(i = (s8)column-1 ; i >=0 ; i -- ) // third probability searching in the same row to the right
   {
     if((Which_Piece(i,row) >0)&&(Which_Piece(i,row)!= E_king))
     {
       Surr[inc]= (i<<4)+row;
       inc++;
       break;
     }
   }
    
   
   for(i = (s8)column+1; i <=7 ; i ++ ) // fourth probability searching in the same row left
   {
        if((Which_Piece(i,row) >0)&&(Which_Piece(i,row) != E_king))
     {
       Surr[inc]= (i<<4)+row;
       inc++;
       break;
     }
   }
   // side movessssssssssssssssssssssssssssssssssssssss
  
     i = (s8)row-1 ;
      //5th probability searching in variant column and row forward right
    
      for(j = (s8)column-1 ; j >=0 ; j -- )
      {
         if((Which_Piece(j,i) >0)&&(Which_Piece(j,i) != E_king))
         {
         Surr[inc]= (j<<4)+i;
         inc++;
         
         break;
         }
         if(i==0)
         {
           break;
         }
         else
         {
            i --;
         }
       }
    
   i = (s8)row+1;
    // 6th probability searching in variant column and row backward left 
   
      for(j = (s8)column+1; j <=7 ; j ++ )
      {
     if((Which_Piece(j,i) >0)&&(Which_Piece(j,i) != E_king))
     {
       Surr[inc]= (j<<4)+i;
       inc++;
       break;
     }
     if(i ==7)
     {
       break;
     }
     else
     {
       
       i ++;
       
     }
   }
     
     i = (s8)row+1;
  
   for(j = (s8)column-1 ; j >=0 ; j -- ) // 7th probability searching in variant column and row backward right
   {
     if((Which_Piece(j,i) >0)&&(Which_Piece(j,i) != E_king))
     {
       Surr[inc]= (j<<4)+i;
       inc++;
       break;
     }
    
     if( i ==7 )
     {
       break;
     }
     else
     {
       i ++; 
     }
   }
  
   i = (s8)row-1 ;
       for(j = (s8)column+1; j <=7 ; j ++ ) // 8th probability searching in variant column and row  forward left
   {
     if((Which_Piece(j,i) >0)&&(Which_Piece(j,i) != E_king))
     {
       Surr[inc]= (j<<4)+i;
       inc++;
       break;
     }
   
     if(i==0)
     {
       break;
     }
     else
     {
       i--;
     }
   
   }
  
   
   
   
  for(i = 0; i<=7 ; i++)
  {
    switch (Which_Piece(Column(Surr[i]),Row(Surr[i])))
 {
  case O_pawn:
    if( (((Column(Surr[i]) - column)==1  )|((column - Column(Surr[i]))==1  )) &&((row-Row(Surr[i]))==1) )
    {
      return 0 ;
    }
    
    break;
  case O_queen:
     
   return 0 ;
     break;
  case O_bishop:
   if((Column(Surr[i])!=column)&&(Row(Surr[i])!=row))
   {
     return 0 ;
   }
    break;
  case O_rook:  
   if((Column(Surr[i])==column)||(Row(Surr[i])==row))
    {
     return 0 ;
    }
   break;   
   case   O_king:
   if(((Column(Surr[i]) - column)==1  )||((column - Column(Surr[i]))==1  )
      
      ||((Row(Surr[i]) - row)==1  )||((row - Row(Surr[i]))==1  ))
   {
     return 0 ;
   }
    break;
  default:
     
  }    
 
  
  
 }
  
// Knight Attacks

if(Which_Piece(column+2 , row+1) == O_knight)
{
return 0 ;  
}
   
if(Which_Piece(column+2 , row-1) == O_knight)
{
return 0 ;  
}

if(Which_Piece(column-2 , row+1) == O_knight)
{
return 0 ;  
}

if(Which_Piece(column-2 , row-1) == O_knight)
{
return 0 ;  
}

   

if(Which_Piece(column+1 , row+2) == O_knight)
{
return 0 ;  
}
   
if(Which_Piece(column+1 , row-2) == O_knight)
{
return 0 ;  
}

if(Which_Piece(column-1 , row+2) == O_knight)
{
return 0 ;  
}

if(Which_Piece(column-1 , row-2) == O_knight)
{
return 0 ;  
}
   
// if it passed all this conditions with no return zero so its safe
   
return 1;

}


void Delay_CC(u64 Counter)
{
  u64 i;
  for(i =0 ; i <= Counter; i++)
  {
    
  } 
}

void Delay_ms(u16 delay_ms)
{
  u16 i;
  for(i=0;i<=delay_ms;i++)
  {
  Delay_CC(16000);
  }
  }