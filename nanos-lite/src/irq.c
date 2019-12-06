#include "common.h"
_Context * do_syscall(_Context *);
static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) 
	{ 
    case _EVENT_YIELD:
		{
			//printf("_EVENT_YIELD is triggered!!\n");
			break;
		}
    case _EVENT_SYSCALL:
		{
			//printf("_EVENT_SYSCALL is triggered!!!\n");
			return do_syscall(c);
		}	
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
