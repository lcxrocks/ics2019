#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1; //sys-call number
  //a[]
  switch (a[0]) {
    //case SYS_exit: _halt(c->GPRx); break;
    case SYS_yield: _yield(); c->GPRx = 0; break;
    case SYS_write: printf("hahsys_write!!\n");break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
