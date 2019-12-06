#include "common.h"
#include "syscall.h"
#include "fs.h"
void sys_yield(_Context *c);
void sys_exit(_Context *c);
void sys_write(_Context *c);
void sys_brk(_Context *c);
void sys_open(_Context *c);
void sys_close(_Context *c);
void sys_read(_Context *c);
void sys_lseek(_Context *c);
void naive_uload(void*, const char*);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
//  printf("a[0] %d\n",a[0]);
  switch (a[0]) {
	case SYS_exit: sys_exit(c); break;
	case SYS_brk: sys_brk(c); break;
    case SYS_write: sys_write(c); break;
    case SYS_yield: sys_yield(c); break;
    case SYS_open: sys_open(c); break;
    case SYS_close: sys_close(c); break; 
	case SYS_read: sys_read(c); break;
	case SYS_lseek: sys_lseek(c); break;
	case SYS_execve: naive_uload(NULL, (char*)c->GPR2); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return c;
}
void sys_exit(_Context *c)
{
   //naive_uload(NULL, "/bin/init");
	printf("exit\n");
	_halt(c->GPRx);
}

void sys_yield(_Context *c)
{
//  printf("sys_yield\n");
   	_yield();
  c->GPRx = 0;
//  do_syscall(c);
 // _halt(0);
}

void sys_write(_Context *c)
{
 /* int fd = c->GPR2;
  char *ptr = (char *)(c->GPR3);
  uint32_t count = c->GPR4;
  Log("%s", ptr);
  if (fd == 1 || fd == 2)
  {
    int i = 0;
    for (; i < count; i++)
      _putc(ptr[i]);
  c->GPRx = i;
  }
  else
    c->GPRx = -1;*/
  c->GPRx = fs_write(c->GPR2, (char *)c->GPR3, c->GPR4);
  return;
}

void sys_brk(_Context *c)
{
  c->GPRx = 0;
  return;
}
void sys_open(_Context *c)
{
  c->GPRx = fs_open((char *)c->GPR2);
}

void sys_close(_Context *c)
{
  c->GPRx = fs_close(c->GPR2);
}

void sys_read(_Context *c)
{
  c->GPRx = fs_read(c->GPR2, (char *)c->GPR3, c->GPR4);
  //printf("gprx: %d\n",c->GPRx);
}

void sys_lseek(_Context *c)
{
  c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
  //printf("gprx: %d gpr1: %d\n", c->GPRx,c->GPR1);
}
