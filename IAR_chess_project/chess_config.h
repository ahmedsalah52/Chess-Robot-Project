

// this section for the pieces values
//ODD FOR THE ROBOT , EVEN FOR THE OPPONENT

#define O_queen  19
#define O_bishop 9
#define O_knight 13
#define O_rook   11
#define O_pawn   3
#define O_king   39 //summition of all pieces value  


#define E_queen   18
#define E_bishop  8
#define E_knight  12
#define E_rook    10
#define E_pawn    2
#define E_king    (O_king - 1)

#define recursion_layers  3    // must be even number
#define opponent_turn     0