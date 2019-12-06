#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  struct _AddressSpace *as;
  //uintptr_t edi,esi,ebp,esp,ebx,edx,ecx,eax; //gprs
  uintptr_t edi;
  uintptr_t esi;
  uintptr_t ebp;
  uintptr_t esp;
  uintptr_t ebx;
  uintptr_t edx;
  uintptr_t ecx;
  uintptr_t eax;
  int irq;
  uintptr_t pc;
  uintptr_t cs;
  uintptr_t eflag;
  
};

#define GPR1 eax
#define GPR2 ebx
#define GPR3 ecx
#define GPR4 edx
#define GPRx eax
//p349 of textbook.
//order: eax \ ebx \ ecx\ edx \ esi \ edi \ ebp
#endif
