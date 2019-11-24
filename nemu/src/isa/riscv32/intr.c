#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //1.push regs
  rtl_push(&cpu.eflags);
  rtl_push(&cpu.cs);
  rtl_push(&cpu.pc);
  //2.read IDT addr
  rtl_li(&s0, cpu.idtr.base);
  //3.search in IDT by NO.
  struct{
    uint32_t offset_l: 16;
    uint32_t :16;
    uint32_t :15;
    uint32_t p: 1;
    uint32_t offset_h:16;
  }_idt_read;
  
  _idt_read idt0 = vaddr_read(idt[NO],8);
  //4. concat offset 
  rtl_shli(&s0,&idt0->offset_h,16);
  rtl_or(&s0,&s0,&idt0->offset_l);
  //5.jmp to s0
  rtl_j(&s0);
  
}

bool isa_query_intr(void) {
  return false;
}
