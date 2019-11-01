#include "cpu/exec.h"

make_EHelper(lidt) {
  TODO();

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
  TODO();

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  TODO();

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
  rtl_host_lm(&s0,&id_src->val,id_src->width);
  switch (id_dest->width)
  {
  case 1:
    s1 = pio_read_b(s0);
    break;
  case 2:
    s1 = pio_read_w(s0);
    break;
  case 4:
    s1 = pio_read_l(s0);
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
  //rtl_host_lm(&s0, &id_dest->val, id_dest->width);
  //rtl_host_lm(&s1, &id_src->val,id_src->width);
  //printf("s0->width: %8x, s1->width: %8x hah\n ", id_dest->width,id_src->width);

  switch(id_dest->width){
    case 1: pio_write_b(id_dest->val,id_src->val); break;
    case 2: pio_write_w(id_dest->val,id_src->val); break;
    case 4: pio_write_l(id_dest->val,id_src->val); break;
    default: printf("func Ehelper_out() down at %d\n", __LINE__); assert(0); break;
  }

  print_asm_template2(out);
}
