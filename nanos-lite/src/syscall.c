#include "common.h"
#include "syscall.h"
int sys_write(int fd, void *buf, size_t count);
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
    case SYS_write: c->GPRx = sys_write((int)a[1],(void *)a[2],(size_t)a[3]);
    //case SYS_brk: a[1]
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