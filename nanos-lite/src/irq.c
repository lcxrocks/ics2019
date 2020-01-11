#include "common.h"
_Context* do_syscall(_Context *c);
_Context* schedule(_Context* prev);
static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
    case _EVENT_YIELD: Log("haha,hit _EVENT_YIELD\n"); 
    return schedule(c);
    break;
    case _EVENT_SYSCALL: return do_syscall(c); break;
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