#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  uintptr_t edi,esi,ebp,esp,ebx,edx,ecx,eax;
  struct _AddressSpace *as;
  int irq;
     /*
   Temp := (SP);
   Push(AX);
   Push(CX);
   Push(DX);
   Push(BX);
   Push(Temp);
   Push(BP);
   Push(SI);
   Push(DI);*/
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
