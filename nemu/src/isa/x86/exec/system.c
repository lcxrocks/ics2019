#include "cpu/exec.h"
void raise_intr(uint32_t NO, vaddr_t ret_addr);
make_EHelper(lidt) {
  //TODO();
  //read a six byte data op(48 bit).
  //16bit: 24+16; //higher order bit not used.
  //32bit: 32+16; 
  if (decinfo.isa.is_operand_size_16) {
    rtl_lm((uint32_t*)&cpu.idtr.limit,&id_dest->addr,2);
    s1=id_dest->addr+2;
    rtl_lm(&s0,&s1,4);
    s1=0x00ffffff;
    rtl_and(&s0,&s0,&s1);
    rtl_lm(&cpu.idtr.base,&s0,4);
  } 
  else {
    s1=id_dest->addr+2;
    rtl_lm((uint32_t*)&cpu.idtr.limit,&id_dest->addr,2);
    rtl_lm(&cpu.idtr.base,&s1,4);
    //printf("limit: 0x%8x    base: 0x%8x\n",cpu.idtr.limit,cpu.idtr.base);
  }
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  raise_intr(id_dest->val, decinfo.seq_pc);

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  //TODO();

  rtl_pop(&cpu.pc);
  printf("cpu.pc : 0x%x\n",cpu.pc);
  //rtl_j(cpu.pc);
  print_asm("iret");
  
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {
  //TODO();
  switch (id_src->width)
  {
  case 1:
    rtl_li(&s1,pio_read_b(id_src->val));
    break;
  case 2:
    rtl_li(&s1,pio_read_w(id_src->val));
    break;
  case 4:
    rtl_li(&s1,pio_read_l(id_src->val));
    break;

  default:
    printf("func Ehelper_in() down at %d\n", __LINE__); 
    assert(0);
    break;
  }
  operand_write(id_dest, &s1);
  print_asm_template2(in);
}

make_EHelper(out) {
  switch(id_dest->width){
    case 1: pio_write_b(id_dest->val,id_src->val); break;
    case 2: pio_write_w(id_dest->val,id_src->val); break;
    case 4: pio_write_l(id_dest->val,id_src->val); break;
    default: printf("func Ehelper_out() down at %d\n", __LINE__); assert(0); break;
  }
  print_asm_template2(out);
}
