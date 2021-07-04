#define GPTMCFG         (*((volatile u32 *)0x4004F000))
#define GPTMTAMR        (*((volatile u32 *)0x4004F004))
#define GPTMTAILR       (*((volatile u32 *)0x4004F028))
#define GPTMTAMATCHR    (*((volatile u32 *)0x4004F030))
#define GPTMCTL         (*((volatile u32 *)0x4004F00C))
#define GPIOPCTL        (*((volatile u32 *)0x4000752C))
#define GPIOAFSEL       (*((volatile u32 *)0x40007420))
#define GPIOAMSEL       (*((volatile u32 *)0x40007528))
#define GPIODEN         (*((volatile u32 *)0x4000751c))
#define RCGCWTIMER      (*((volatile u32 *)0x400FE65c))
#define RCGCGPIO        (*((volatile u32 *)0x400FE608))
      

