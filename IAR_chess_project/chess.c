#include"macros.h"
#include"std_types.h"
#include"chess_config.h"
#include"Robot_interface.h"

#include"Chess_interface.h"



void Robots_Turn(void)
{
  s64 Best_Score , Score;
  u8 equals, Best_Move ,j, i;
  Best_Move = 0;
  Score =0;
  Best_Score = -16000000;
  i = 0;
  equals = 0;
  j =0;
  MOVE Props[200]={0};
  u8 equal_moves[50] = {0};
  prob_num(Props , 1 ); // this function counts the probabilities and save each on in the probabilities array , robots turn
  
  General_Best_score = 0;

  while(1)
  {
    if(((Props[i].Next_Position) == 0)&&(Props[i].Initial_Position == 0))
    {
      break;
    }
    
    
    if((Row(Props[i].Next_Position)>7) || (Column(Props[i].Next_Position)>7))
    {
      Props[i].Next_Position = 0;
      Props[i].Next_Position = 0;
      i++;
    }
    else
    {
    Score =  chess_board[Row(Props[i].Next_Position)][Column(Props[i].Next_Position)]; // add the value of the next pos piece if exists
    
    Score -= Calculate_Score(Props[i] , recursion_layers , opponent_turn , Score); //predect the score of the next expected moves
    
     if(Score == Best_Score)
     {
       equal_moves[equals] = i;
       equals++;       
     }
     else if(Score > Best_Score)
     {
       for(j = 0;j<50;j++)
       {
         equal_moves[j] = 0;  
       }
       equals = 0;
       Best_Score = Score ;
       
    
       equal_moves[equals] = i;
       
       equals++;       
     }
   
   
    i++;
    }
    
  }
  
    Best_Move = equal_moves[equals/2];
  chess_board[Row(Props[Best_Move].Next_Position)][Column(Props[Best_Move].Next_Position)]
    = chess_board[Row(Props[Best_Move].Initial_Position)][Column(Props[Best_Move].Initial_Position)] ; //make the move in the arr
  
  chess_board[Row(Props[Best_Move].Initial_Position)][Column(Props[Best_Move].Initial_Position)] =0 ;
  
  board_inputs[Row(Props[Best_Move].Initial_Position)] &= (~(1<<Column(Props[Best_Move].Initial_Position)));
   
  board_inputs[Row(Props[Best_Move].Next_Position)] |= (1)<<Column(Props[Best_Move].Next_Position);
  
  
  Robots_Move(Props[Best_Move].Initial_Position ,Props[Best_Move].Next_Position );
  
}

u8 prob_num(MOVE * Probs , u8 Robots_Turn)
{
  u8 Init_Pos,column,row, i;
  i = 0;
  
  for(row = 0 ; row <=7 ; row ++)
  {
     for(column = 0 ; column <=7 ; column ++)
     { 
       Init_Pos = (column<<4)+row;     
       switch ((chess_board[row][column]) - Robots_Turn)
       {
         case 0:
         
         break;
         case  E_pawn: 
             
           i =  Pawn( Probs ,Init_Pos, Robots_Turn ,i);
         
         break;
         
         case  E_queen: 
         
           i =Queen( Probs ,Init_Pos, Robots_Turn ,i);
         
         break;
         
         case  E_bishop: 
         
           i = Bishop( Probs ,Init_Pos, Robots_Turn ,i);
         
         break;
         
         case  E_knight: 
          i =   Knight( Probs ,Init_Pos, Robots_Turn ,i);
         
         
         break;
         
         case  E_rook: 
            i = Rock( Probs ,Init_Pos, Robots_Turn ,i);
         
         break;
         
         case  E_king: 
         
           i =King( Probs ,Init_Pos, Robots_Turn ,i);
         
         break;
       }
     }   
  }

return i;
}

s64 Calculate_Score(MOVE Move_Test , u8 Recursion_Layers , u8 Robots_Turn , s64 old_score )
{
  
  if(Recursion_Layers == 0)
  {
  
    return 0;
  }

  u8 temp , i;
  i = 0;  
  u8 Kill_Counter =0;
  Score_of Score ;
  
  s64 diff_score ,  Best_Score ; 
  Best_Score = -1000000;
  diff_score =0;
  Score.robot = 0;
  Score.opponent =0;
  
  
    
  // make the move temprorly to calculate the score
  temp =chess_board[Row(Move_Test.Next_Position)][Column(Move_Test.Next_Position)] ;
  
  chess_board[Row(Move_Test.Next_Position)][Column(Move_Test.Next_Position)] 
    = chess_board[Row(Move_Test.Initial_Position)][Column(Move_Test.Initial_Position)];
  
  chess_board[Row(Move_Test.Initial_Position)][Column(Move_Test.Initial_Position)] = 0;
  
 
  MOVE Props[200]={0};
  
  
  prob_num(Props , Robots_Turn ); // this function counts the probabilities and save each on in the probabilities array , robots turn
  
  while(1)
  {
    if(((Props[i].Next_Position) == 0)&&(Props[i].Initial_Position == 0))
    {
      break;
    }
    
    
    if((Row(Props[i].Next_Position)>7) || (Column(Props[i].Next_Position)>7))
    {
      Props[i].Next_Position = 0;
      Props[i].Next_Position = 0;
      i++;
    }
    else
    {
            
      if(Props[i].Next_Position ==Move_Test.Next_Position)
      {
        Kill_Counter++;
      
      }
      
      if(Robots_Turn == opponent_turn)
      {
          
          Score.opponent =  chess_board[Row(Props[i].Next_Position)][Column(Props[i].Next_Position)]; // add the value of the next pos piece if exists
          
          Score.robot = Calculate_Score(Props[i] , Recursion_Layers-1 , !Robots_Turn , (old_score - Score.opponent) ); //predect the score of the next expected moves
          diff_score = Score.opponent - Score.robot;
        
      }
      else
      {
          Score.robot =  chess_board[Row(Props[i].Next_Position)][Column(Props[i].Next_Position)]; // add the value of the next pos piece if exists
          
          if ((Recursion_Layers >= recursion_layers/2)&&(General_Best_score > (3*(old_score + Score.robot ))))
          {
             return (old_score + Score.robot );
          }
          
          Score.opponent = Calculate_Score(Props[i] , Recursion_Layers-1 , !Robots_Turn , (old_score + Score.robot )); //predect the score of the next expected moves
          diff_score = Score.robot - Score.opponent;
          
      }
      
      
      
      if(diff_score > Best_Score)
          {
            Best_Score = diff_score;
          }

      if ((Recursion_Layers == 1)&&(General_Best_score > old_score))//last layer
          {
              General_Best_score  = old_score;
          }
  
    i++;
    }  
  }
  
  
   // after the calculations return the move again then return the score
 chess_board[Row(Move_Test.Initial_Position)][Column(Move_Test.Initial_Position)]
=  chess_board[Row(Move_Test.Next_Position)][Column(Move_Test.Next_Position)] ;

 chess_board[Row(Move_Test.Next_Position)][Column(Move_Test.Next_Position)]  = temp ;

  return  Best_Score;
}


u8 Pawn(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i)
{
  u8 row_move , column_move;
  
  if(Robots_Turn == 1) //robot
  {
  row_move = 0x01 ; // +( !Robots_Turn )*2;
  column_move = 0x10 ;
  
  
  if((Row(Init_Pos) == 1) && (chess_board[Row(Init_Pos)+(row_move)][Column(Init_Pos)] ==0)
     &&(chess_board[Row(Init_Pos)+(2*row_move)][Column(Init_Pos)] ==0) )
  {
    Probs[i].Initial_Position = Init_Pos;    
    Probs[i].Next_Position = Init_Pos + (2*row_move);
    i++;      
  }
  
  if(chess_board[Row(Init_Pos)+(row_move)][Column(Init_Pos)] ==0)
  {
    Probs[i].Initial_Position = Init_Pos;
    Probs[i].Next_Position = Init_Pos + (row_move);
    i++;      
  }
 
  
  
  if(((chess_board[Row(Init_Pos)+(row_move)][Column(Init_Pos+column_move)]%2) == 0) // ensure that its even (num )opponent)
     &&(chess_board[Row(Init_Pos)+(row_move)][Column(Init_Pos+column_move)]) != 0) // not empty
  {
    Probs[i].Initial_Position = Init_Pos;
    Probs[i].Next_Position = Init_Pos + row_move +column_move;
    i++;      
  }
 
  
  if(((chess_board[Row(Init_Pos)+(row_move)][Column(Init_Pos-column_move)]%2) == (!Robots_Turn)) // ensure that its not ur piece
     &&(chess_board[Row(Init_Pos)+(row_move)][Column(Init_Pos-column_move)]) != 0) // not empty
  {
    Probs[i].Initial_Position = Init_Pos;   
    Probs[i].Next_Position = Init_Pos + row_move -column_move;
    i++;      
  }
  }
  else
  {
  row_move = 0x01 ; // +( !Robots_Turn )*2;
  column_move = 0x10 ;
  
  
  if((Row(Init_Pos) == 6)
  && (chess_board[Row(Init_Pos)-(row_move)][Column(Init_Pos)] ==0)
    &&(chess_board[Row(Init_Pos)-(2*row_move)][Column(Init_Pos)] ==0) )
  {
    Probs[i].Initial_Position = Init_Pos;    
    Probs[i].Next_Position = Init_Pos - (2*row_move);
    i++;      
  }
  
  if(chess_board[Row(Init_Pos)-(row_move)][Column(Init_Pos)] ==0)
  {
    Probs[i].Initial_Position = Init_Pos;
    Probs[i].Next_Position = Init_Pos - (row_move);
    i++;      
  }
 
  
  
  if((chess_board[Row(Init_Pos)-(row_move)][Column(Init_Pos+column_move)]%2) == 1) // ensure that its robots piece
      // not empty
  {
    Probs[i].Initial_Position = Init_Pos;
    Probs[i].Next_Position = Init_Pos -row_move +column_move;
    i++;      
  }
 
  
  if((chess_board[Row(Init_Pos)-(row_move)][Column(Init_Pos-column_move)]%2) == 1) // ensure that its not ur piece
     // not empty
  {
    Probs[i].Initial_Position = Init_Pos;   
    Probs[i].Next_Position = Init_Pos - row_move -column_move;
    i++;      
  }
    
  }
  return i;
}


u8 Queen(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i)
{
   s8 row , column;
   
   for(row = (Row(Init_Pos)-1) ; row >=0 ; row--)
   {
     if((chess_board[row][Column(Init_Pos)] == 0 ) || ((chess_board[row][Column(Init_Pos)]%2) == (!Robots_Turn) ))
     {//empty save as prob or other side hunt
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = (Column(Init_Pos)<<4) + row;
        i++;      
       
      if((chess_board[row][Column(Init_Pos)]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][Column(Init_Pos)]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }  
   for(row = (Row(Init_Pos)+1) ; row <=7 ; row++)
   {
     if((chess_board[row][Column(Init_Pos)] == 0 ) || ((chess_board[row][Column(Init_Pos)]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = (Column(Init_Pos)<<4) + row;
        i++;      

      if((chess_board[row][Column(Init_Pos)]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[row][Column(Init_Pos)]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }

   
     for(column = (Column(Init_Pos)+1) ; column <=7 ; column++)
   {
     if((chess_board[Row(Init_Pos)][column] == 0 ) || ((chess_board[Row(Init_Pos)][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = Row(Init_Pos) + (column<<4);
        i++;      
     
        if((chess_board[Row(Init_Pos)][column]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[Row(Init_Pos)][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }
 
   for(column = (Column(Init_Pos)-1) ; column >=0 ; column--)
   {
     if((chess_board[Row(Init_Pos)][column] == 0 ) || ((chess_board[Row(Init_Pos)][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = Row(Init_Pos)+ (column<<4);
        i++;      
        
        if((chess_board[Row(Init_Pos)][column]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[Row(Init_Pos)][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }
 
// as a bishop
   column = Column(Init_Pos)-1;
   for(row = (Row(Init_Pos)-1) ; row >=0 ; row--)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position =  row +(column<<4);
        i++;      
        if((chess_board[row][column]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
    
     if(column > 0)
     {
      column --;
     }
     else
     {
       break;
     }
   
   }  
  
    column = Column(Init_Pos)+1;
  
   for(row = (Row(Init_Pos)+1) ; row <=7 ; row++)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = row +(column<<4);
        i++; 
        
      if((chess_board[row][column]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
  
     
     if(column < 7)
     {
      column ++;
     }
     else
     {
       break;
     }
  
   
   }

   column = Column(Init_Pos)+1;
   for(row = (Row(Init_Pos)-1) ; row >=0 ; row--)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position =  row +(column<<4);
        i++;      

     if((chess_board[row][column]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
    
     if(column < 7)
     {
      column ++;
     }
     else
     {
       break;
     }
   
   }  
  
    column = Column(Init_Pos)-1;
  
   for(row = (Row(Init_Pos)+1) ; row <=7 ; row++)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = row +(column<<4);
        i++;      

     if((chess_board[row][column]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
  
     
     if(column > 0)
     {
      column --;
     }
     else
     {
       break;
     }
  
   
   }

     
   return i;
   }
    

u8 Rock(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i)
{
   s8 row , column;
   
   for(row = (Row(Init_Pos)-1) ; row >=0 ; row--)
   {
     if((chess_board[row][Column(Init_Pos)] == 0 ) || ((chess_board[row][Column(Init_Pos)]%2) == (!Robots_Turn) ))
     {//empty save as prob or other side hunt
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = (Column(Init_Pos)<<4) + row;
        i++;      
       
      if((chess_board[row][Column(Init_Pos)]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][Column(Init_Pos)]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }  
   for(row = (Row(Init_Pos)+1) ; row <=7 ; row++)
   {
     if((chess_board[row][Column(Init_Pos)] == 0 ) || ((chess_board[row][Column(Init_Pos)]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = (Column(Init_Pos)<<4) + row;
        i++;      

      if((chess_board[row][Column(Init_Pos)]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[row][Column(Init_Pos)]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }

   
     for(column = (Column(Init_Pos)+1) ; column <=7 ; column++)
   {
     if((chess_board[Row(Init_Pos)][column] == 0 ) || ((chess_board[Row(Init_Pos)][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = Row(Init_Pos) + (column<<4);
        i++;      
     
        if((chess_board[Row(Init_Pos)][column]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[Row(Init_Pos)][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }
 
   for(column = (Column(Init_Pos)-1) ; column >=0 ; column--)
   {
     if((chess_board[Row(Init_Pos)][column] == 0 ) || ((chess_board[Row(Init_Pos)][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = Row(Init_Pos)+ (column<<4);
        i++;      
        
        if((chess_board[Row(Init_Pos)][column]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[Row(Init_Pos)][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
   }
 

     
   return i;
   }
    

u8 Bishop(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i)
{
  
// as a bishop
  s8 column , row; 
  
  column = Column(Init_Pos)-1;
   for(row = (Row(Init_Pos)-1) ; row >=0 ; row--)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position =  row +(column<<4);
        i++;      
        if((chess_board[row][column]) >0  )
        {
          break;
        }

     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
    
     if(column > 0)
     {
      column --;
     }
     else
     {
       break;
     }
   
   }  
  
    column = Column(Init_Pos)+1;
  
   for(row = (Row(Init_Pos)+1) ; row <=7 ; row++)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = row +(column<<4);
        i++; 
        
      if((chess_board[row][column]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
  
     
     if(column < 7)
     {
      column ++;
     }
     else
     {
       break;
     }
  
   
   }

   column = Column(Init_Pos)+1;
   for(row = (Row(Init_Pos)-1) ; row >=0 ; row--)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position =  row +(column<<4);
        i++;      

     if((chess_board[row][column]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
    
     if(column < 7)
     {
      column ++;
     }
     else
     {
       break;
     }
   
   }  
  
    column = Column(Init_Pos)-1;
  
   for(row = (Row(Init_Pos)+1) ; row <=7 ; row++)
   {
     if((chess_board[row][column] == 0 ) || ((chess_board[row][column]%2) == (!Robots_Turn) ))
     {//empty save as prob
        
        Probs[i].Initial_Position = Init_Pos;
        Probs[i].Next_Position = row +(column<<4);
        i++;      

     if((chess_board[row][column]) >0  )
        {
          break;
        }

        
     }
     else if ((chess_board[row][column]%2) == Robots_Turn ) // his piece , odd for robot ,even for opponent
     {
       break;
     }
  
     
     if(column > 0)
     {
      column --;
     }
     else
     {
       break;
     }
  
   
   }
   
   return i;
   }


u8 Knight(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i)
{  u8 j ;

    u8 Moves[8] =
    {Init_Pos +0x10 + 0x02
   ,Init_Pos +0x10 - 0x02
   ,Init_Pos -0x10 + 0x02
   ,Init_Pos -0x10 - 0x02
   ,Init_Pos +0x20 + 0x01
   ,Init_Pos +0x20 - 0x01
   ,Init_Pos -0x20 + 0x01
   ,Init_Pos -0x20 - 0x01};
     
     
   for(j = 0 ; j<8 ; j++)
   {
     if((Column(Moves[j]) >7)||(Row(Moves[j])>7))
     {
      
     }
     else
     {
       if((chess_board[Row(Moves[j])][Column(Moves[j])] == 0 ) || ((chess_board[Row(Moves[j])][Column(Moves[j])]%2) == (!Robots_Turn) ))
        {
             Probs[i].Initial_Position = Init_Pos;
             Probs[i].Next_Position = Moves[j];
             i++;      
    
        }
     }
   }

return i;
}


u8 King(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i)
{
    u8 j ;

    u8 Moves[8] =
   {Init_Pos +0x10
   ,Init_Pos -0x10
   ,Init_Pos +0x01
   ,Init_Pos -0x01
   ,Init_Pos +0x10 + 0x01
   ,Init_Pos +0x10 - 0x01
   ,Init_Pos -0x10 + 0x01
   ,Init_Pos -0x10 - 0x01};
     
     
   for(j = 0 ; j<8 ; j++)
   {
     if((Column(Moves[j]) >7)||(Row(Moves[j])>7))
     {
      
     }
     else
     {
       if((chess_board[Row(Moves[j])][Column(Moves[j])] == 0 )
         || (((chess_board[Row(Moves[j])][Column(Moves[j])])%2) == (!Robots_Turn) ))
        {
             Probs[i].Initial_Position = Init_Pos;
             Probs[i].Next_Position = Moves[j];
             i++;      
    
        }
     }
   }

return i;
}
