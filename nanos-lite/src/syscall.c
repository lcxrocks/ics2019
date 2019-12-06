#include "common.h"
#include "syscall.h"
int fs_open(const char *path,int flags,int mode);
int fs_close(int fd);
int fs_read(int fs,void *buf,size_t len);
int fs_write(int fd,void *buf,size_t len);
__off_t fs_lseek(int fd,__off_t offset,int whence);

int sys_yield()
{
	_yield();
	return 0;
}

int sys_brk()
{
	return 0;
}

_Context* do_syscall(_Context *c) 
{
  uintptr_t a[4];
  a[0]=c->GPR1;
	a[1]=c->GPR2;
  a[2]=c->GPR3;
	a[3]=c->GPR4;

  switch (a[0]) 
	{
  	case SYS_exit:
		{
      //printf("I have reached here in do_syscall SYS_exit!!!\n");
      _halt(a[1]);
      break;
		}
		case SYS_yield:
		{
      //printf("I have reached here in do_syscall SYS_yield!!!\n");
      c->GPRx=sys_yield();
		  break;		
		}
		case SYS_open:
		{
      //printf("I have reached here in do_syscall SYS_open!!!\n");
      c->GPRx=fs_open((const char *)a[1],(int)a[2],(int)a[3]); 
			break;
		}
		case SYS_read:
		{
      //printf("I have reached here in do_syscall SYS_read!!!\n");
			c->GPRx=fs_read((int)a[1],(void *)a[2],(size_t)a[3]);
			break;
		}
		case SYS_close:
		{
      //printf("I have reached here in do_syscall SYS_close!!!\n");
			c->GPRx=fs_close((int)a[1]);
			break;
		}
		case SYS_lseek:
		{
      //printf("I have reached here in do_syscall SYS_lseek!!!\n");
			c->GPRx=fs_lseek((int)a[1],(__off_t)a[2],(int)a[3]);
			break;
		}
		case SYS_brk:
		{
      //printf("I have reached here in do_syscall SYS_brk!!!\n");
      c->GPRx=sys_brk();
			break;
		}
		case SYS_write:
		{	
			//printf("a1 is %d and a2 is %d and a3 is %d\n",a[1],a[2],a[3]);
			c->GPRx=fs_write((int)a[1],(char *)a[2],(int)a[3]);
			//printf("the cnt is %d\n",c->GPRx);
      //printf("I have reached here in do_syscall SYS_write!!!\n");
			break;
		}
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
