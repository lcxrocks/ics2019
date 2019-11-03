#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  //TODO();
  rtl_and(&s0, &id_dest->val,&id_src->val);
  rtl_update_ZFSF(&s0,id_dest->width);

  s0 = 0;
  rtl_set_CF(&s0);
  rtl_set_OF(&s0);
  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&s0, &id_dest->val, &id_src->val);
  //printf("s0: %8x; id_dest: %8x; id_src: %8x",s0,id_dest->val,id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);

  s0=0;
  rtl_set_OF(&s0);
  rtl_set_CF(&s0);
  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&s0,&id_dest->val, &id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);

  s0=0;
  rtl_set_CF(&s0);
  rtl_set_OF(&s0);
  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&s0,&id_dest->val, &id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);
  
  s0=0;
  rtl_set_CF(&s0);
  rtl_set_OF(&s0);
  print_asm_template2(or);
}

make_EHelper(sar) {
 // printf("src: %8x; dest %8x, width: %8x \n",id_src->val,id_dest->val,id_dest->width);
  rtl_sext(&id_dest->val,&id_dest->val,id_dest->width);
  rtl_sar(&s0, &id_dest->val, &id_src->val);
  //printf("s0: %8x; src: %8x; dest %8x\n",s0, id_src->val,id_dest->val);
  operand_write(id_dest, &s0);
  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&s0,id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  rtl_shl(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&s0,id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtl_shr(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  // unnecessary to update CF and OF in NEMU
  rtl_update_ZFSF(&s0,id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {//dest,src
  
  rtl_not(&s0, &id_dest->val);
  operand_write(id_dest, &s0);
  print_asm_template1(not);
}

make_EHelper(rol){
  rtl_host_lm(&s0, &id_dest->val, id_dest->width);
  rtl_host_lm(&s1, &id_src->val, id_dest->width);
  uint32_t topdig = 0;
  for (; s1 > 0; s1--){
    rtl_msb(&topdig, &s0, id_dest->width);
    rtl_shli(&s0, &s0, 1);
    rtl_or(&s0, &s0, &topdig);
  }
  operand_write(id_dest, &s0);
  if (s1 == 1){
    rtl_get_CF(&s1);
    rtl_msb(&topdig, &s0, id_dest->width);
    rtl_xor(&s1, &s1, &topdig);
    rtl_set_OF(&s1);
  }
  print_asm_template2(rol);
  /*
  uint32_t count = 0;
  rtl_host_lm(&count,&id_dest->val,id_dest->width); //count
  assert(count != 0);
  if (count == 1){
    rtl_msb(&s1, &id_dest->val, id_dest->width);
    if(s1 != cpu.CF) cpu.OF=1;
    else cpu.OF = 0;
  }

  while(count != 0){
    rtl_msb(&s0, &id_dest->val, id_dest->width);
    id_dest->val = 2*id_dest->val + s0;
    //printf("id_dest->width: %8x\n",id_dest->width);
    switch (id_dest->width)
    {
    case 1:
      id_dest->val &= 0xFF;
      break;
    case 2:
      id_dest->val &= 0xFFFF;
      break;
    case 4:
      id_dest->val &= 0xFFFFFFFF;
      break;
    default:
      printf("rol down at line %d\n",__LINE__);
      assert(0);
      break;
    }
    count--;
  }
  operand_write(id_dest, &id_dest->val);
  print_asm_template2(rol);  */
}
