

#define PORT_A    0
#define PORT_B    1
#define PORT_C    2 
#define PORT_D    3
#define PORT_E    4
#define PORT_F    5

#define PIN_0     0
#define PIN_1     1
#define PIN_2     2
#define PIN_3     3
#define PIN_4     4
#define PIN_5     5
#define PIN_6     6
#define PIN_7     7

#define HIGH      1
#define LOW       0

#define OUTPUT    1
#define INPUT     0


#define EN_PULLUP    1
#define DIS_PULLUP   0

void GPIO_voidSetPinDir(u8 PORT , u8 PIN , u8 Dir);
void GPIO_voidSetInputPin_PR(u8 PIN , u8 Pull_up); // enable pull up 1
void GPIO_voidSetPinValue(u8 PORT , u8 Pin , u8 Value);
u8 GPIO_u8GetPinValue(u8 PORT , u8 Pin );