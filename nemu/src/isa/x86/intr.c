#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  /*
  printf("int: 0x%8x\n", NO);
  printf("pc: 0x%8x\n", cpu.pc);
  printf("cpu.idtr.base: 0x%8x\n", cpu.idtr.base);
  printf("cpu.idtr.limit:0x%8x\n", cpu.idtr.limit);
  */
  //1.push regs
  rtl_push(&cpu.init);//eflags
  rtl_push(&cpu.cs);
  rtl_push(&cpu.pc);
  //2.read IDT addr
  vaddr_t idt_addr;
  rtl_li(&idt_addr, cpu.idtr.base);
  //3.search in IDT by NO.
  s0 = vaddr_read(idt_addr+8*NO,2); //low_addr
  s1 = vaddr_read(idt_addr+8*NO+6,2); //high_addr
  //4. concat offset 
  //printf("s1:  0x%8x\n",s1);
  rtl_shli(&s1,&s1,16);
  //printf("s1:  0x%8x\n",s1);
  rtl_or(&s0,&s0,&s1);
  //5.jmp to s0
  //printf("s0: 0x%8x\n",s0);
  rtl_j(s0);
  //didn't set the limit, could be a bug. ---lcx 11.23
}

bool isa_query_intr(void) {
  return false;
}
