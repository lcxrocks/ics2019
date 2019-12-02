#include "common.h"
_Context* do_syscall(_Context *c);
static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
    case _EVENT_YIELD: Log("haha,hit _EVENT_YIELD\n"); 
      printf("-----context \% edi: 0x%x----\n",c->edi);
  printf("-----context \% esi: 0x%x----\n",c->esi);
  printf("-----context \% ebp: 0x%x----\n",c->ebp);
  printf("-----context \% esp: 0x%x----\n",c->esp);
  printf("-----context \% ebx: 0x%x----\n",c->ebx);
  printf("-----context \% edx: 0x%x----\n",c->edx);
  printf("-----context \% ecx: 0x%x----\n",c->ecx);
  printf("-----context \% eax: 0x%x----\n",c->eax);
  printf("-----context c.irq : 0x%x----\n",c->irq);
    
    break;
    case _EVENT_SYSCALL: printf("syscall!\n"); do_syscall(c); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}


/*
enum {
  _EVENT_NULL = 0,
  _EVENT_ERROR = 1
  _EVENT_IRQ_TIMER = 2,
  _EVENT_IRQ_IODEV = 3,
  _EVENT_PAGEFAULT = 4,
  _EVENT_YIELD = 5,
  _EVENT_SYSCALL = 6, 
};
*/