
extern u8 board_inputs[8];

u8 chess_board[8][8] = // row // column
{{O_rook,O_knight,O_bishop,O_king,O_queen,O_bishop,O_knight,O_rook} //robots side
,{O_pawn,O_pawn  ,O_pawn  ,O_pawn,O_pawn ,O_pawn  ,O_pawn  ,O_pawn}
,{0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0,0}
,{0,0,0,0,0,0,0,0}
,{E_pawn,E_pawn  ,E_pawn  ,E_pawn,E_pawn ,E_pawn  ,E_pawn  ,E_pawn}
,{E_rook,E_knight,E_bishop,E_king,E_queen,E_bishop,E_knight,E_rook}  //opponent side
}; // rows , columns  

typedef struct Move
{
  u8 Initial_Position;
  u8 Next_Position;
}MOVE;


typedef struct SCORE
{
  u64 robot;
  u64 opponent;
}Score_of;

u64 General_Best_score;

u8 prob_num(MOVE * Probs , u8 Robots_Turn);

void Robots_Turn(void);

s64 Calculate_Score(MOVE Move_Test , u8 Recursion_Layers , u8 Robots_Turn , s64 old_score );
u8 Pawn(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i);
u8 Queen(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i);

u8 Rock(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i);
u8 Bishop(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i);
u8 Knight(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i);
u8 King(MOVE * Probs ,u8 Init_Pos,  u8 Robots_Turn , u8 i);
//void Mirror_Board(void);