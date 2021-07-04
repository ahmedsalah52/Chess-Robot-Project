#define GPTMCFG         (*((volatile u32 *)0x40037000))
#define GPTMTAMR        (*((volatile u32 *)0x40037004))
#define GPTMTAILR       (*((volatile u32 *)0x40037028))
#define GPTMTAMATCHR    (*((volatile u32 *)0x40037030))
#define GPTMCTL         (*((volatile u32 *)0x4003700C))
#define GPIOPCTL        (*((volatile u32 *)0x4000652C))
#define GPIOAFSEL       (*((volatile u32 *)0x40006420))
#define GPIOAMSEL       (*((volatile u32 *)0x40006528))
#define GPIODEN         (*((volatile u32 *)0x4000651c))
#define RCGCWTIMER      (*((volatile u32 *)0x400FE65c))
#define RCGCGPIO        (*((volatile u32 *)0x400FE608))
      

