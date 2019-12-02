#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  struct _AddressSpace *as;
  uintptr_t edi,esi,ebp,esp,ebx,edx,ecx,eax; //gprs
  int irq;
  uintptr_t pc;
  uintptr_t cs;
  uintptr_t eflag;
  
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eax

#endif
