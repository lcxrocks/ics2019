#include "common.h"
#include "syscall.h"
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1; //sys-call type
  a[1] = c->GPR2; //arg1: fd
  a[2] = c->GPR3; //arg2: buf
  a[3] = c->GPR4; //arg3: count
  //
  printf("--------------a[0]: %x--------------\n",a[0]);
  switch (a[0]) {
    case SYS_exit: _halt(c->GPRx); break;
    case SYS_yield: _yield(); c->GPRx = 0; break;
    case SYS_write: if(a[1]==1||a[1]==2) //stdout/stderr
                    { 
                      printf("sys_write !!!!\n");
                      int cnt = 0;
                      for (int i = 0; i < a[3]; i++)
                      {
                        _putc(((const char*)a[1])[i]);
                        cnt++;
                      }
                      c->GPRx = cnt;
                    }            
                    else
                    {
                      c->GPRx = -1;
                    }
                    
                    break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
