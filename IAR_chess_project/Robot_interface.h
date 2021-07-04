void Go_Position(u8 Row,u8 Column , u8 Hight );

void Check_Board(void);
void Robot_INIT(void);
u8 Check_Move(u8*Moves);
u8 Which_Piece(u8 x , u8 y );
u8 Check_Legal_Move(u8 Piece_number , u8 Initial_POS, u8 Next_POS);
void Start_Game(void);
u8 Make_Move(u8 Initial_Pos , u8 Next_POS);
u8 Row(u8 Position);
u8 Column(u8 Position);
void Wrong_Move(void);
void Right_Move(void);
u8 E_Pawn( u8 Initial_POS, u8 Next_POS);
u8 E_Queen( u8 Initial_POS, u8 Next_POS);
u8 E_Bishop( u8 Initial_POS, u8 Next_POS);
u8 E_Rook( u8 Initial_POS, u8 Next_POS);
u8 E_Knight( u8 Initial_POS, u8 Next_POS);
u8 E_King( u8 Initial_POS, u8 Next_POS);
u8 CheckMate(u8 column , u8 row);
u8 Check_Cell(u8 column , u8 row);
void Robots_Move(u8 Init_Pos , u8 Next_Pos);
void Release_Gripper(void);
void Open_Gripper(void);
void Close_Gripper(void);
void Delay_CC(u64 Counter);
void Delay_ms(u16 delay_ms);
void Set_Robot_Angles(u16 Base_Angle , u16 Main_Degree , u16 Arm_Degree);
void Make_Robot_Move(u8 Init_Pos , u8 Next_Pos);