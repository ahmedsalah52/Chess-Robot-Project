#define GPTMCFG         (*((volatile u32 *)0x40040000))
#define GPTMTAMR        (*((volatile u32 *)0x40040004))
#define GPTMTAILR       (*((volatile u32 *)0x40040028))
#define GPTMTAMATCHR    (*((volatile u32 *)0x40040030))
#define GPTMCTL         (*((volatile u32 *)0x4004000C))
#define GPIOPCTL        (*((volatile u32 *)0x4000552C))
#define GPIOAFSEL       (*((volatile u32 *)0x40005420))
#define GPIOAMSEL       (*((volatile u32 *)0x40005528))
#define GPIODEN         (*((volatile u32 *)0x4000551c))
#define RCGCTIMER       (*((volatile u32 *)0x400FE604)) 
#define RCGCGPIO        (*((volatile u32 *)0x400FE608))
      

