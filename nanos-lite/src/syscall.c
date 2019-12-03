#include "common.h"
#include "syscall.h"
int sys_write(int fd, void *buf, size_t count);
void *sys_brk(intptr_t increment); 
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1; //sys-call type
  a[1] = c->GPR2; //arg1: 
  a[2] = c->GPR3; //arg2: 
  a[3] = c->GPR4; //arg3: 
  //
  //printf("--------------a[0]: %x--------------\n",a[0]);
  switch (a[0]) {
    case SYS_exit: _halt(c->GPRx); break;
    case SYS_yield: _yield(); c->GPRx = 0; break;
    //case SYS_write: c->GPRx = sys_write((int)a[1],(void *)a[2],(size_t)a[3]); break;
    case SYS_brk: c->GPRx = (uintptr_t) sys_brk((intptr_t)a[1]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

int sys_write(int fd, void *buf, size_t count)
{
  if(fd==1||fd==2) //stdout/stderr
  { 
    int cnt = 0;
    for (int i = 0; i < count; i++)
    {
       _putc(((const char*)buf)[i]);
       cnt++;
    }
    return cnt;
  }            
  else
  return -1;
}

extern intptr_t _end; //must have a type, or gcc complains.

void *sys_brk(intptr_t increment)
{
  if(increment == 0) return (void *)_end;
  else 
  {
    intptr_t pre_end = _end;
    _end += increment;
    return (void *) pre_end;
  }
  return (void *)-1;
}
