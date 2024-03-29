#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"
int sys_write(int fd, void *buf, size_t count);
void naive_uload(PCB *pcb, const char *filename);
intptr_t sys_brk(intptr_t increment); 
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1; //sys-call type
  a[1] = c->GPR2; //arg1: 
  a[2] = c->GPR3; //arg2: 
  a[3] = c->GPR4; //arg3: 
  
  //printf("--------------a[0]: %x--------------\n",a[0]);
  switch (a[0]) {
    case SYS_exit: _halt(c->GPRx); break;
    case SYS_yield: _yield(); c->GPRx = 0; break;
    case SYS_open: c->GPRx = fs_open((char *)a[1], (int)a[2], (int) a[3]); break;
    case SYS_read: c->GPRx = fs_read((int)a[1],(void *)a[2],(size_t)a[3]); break;
    case SYS_write: c->GPRx = fs_write((int)a[1],(void *)a[2],(size_t)a[3]); break;
    case SYS_close: c->GPRx = fs_close((int)a[1]); break;
    case SYS_lseek: c->GPRx = fs_lseek((int)a[1],(size_t)a[2],(int)a[3]); break;
    case SYS_brk: c->GPRx = 0; break;
    case SYS_execve: naive_uload(NULL,(char *)a[1]);break;
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
