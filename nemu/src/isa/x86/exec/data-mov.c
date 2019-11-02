#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&s0);
  operand_write(id_dest, &s0);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  
  if(id_dest->width==2){
    rtl_lr(&s0,R_EBP,2);
    rtl_sr(R_ESP,&s0,2);
  }
  else{
    rtl_lr(&s0,R_EBP,4);
    rtl_sr(R_ESP,&s0,4);
  }
  if (decinfo.isa.is_operand_size_16) {
    rtl_pop(&s0);
    rtl_sr(R_BP,&s0,2);
    //BP := Pop();
  }
  else { //* OperandSize = 32 *
    rtl_pop(&s0);
    rtl_sr(R_BP,&s0,4);
    //EBP := Pop();
  }
  print_asm("leave");
}

make_EHelper(cltd) {
  //*dest,*src,width
  s1=cpu.eax;
  rtl_msb(&s0, &s1, 2);
  rtl_msb(&s1, &s1, 4);
  if (decinfo.isa.is_operand_size_16) {//cwd
    if(s0==1)
        cpu.gpr[R_DX]._16=0xFFFF;
    else 
        cpu.gpr[R_DX]._16=0x0000;
  }
  else {//cdq
    if(s1==1)
        cpu.edx=0xFFFFFFFF;
    else
        cpu.edx=0x00000000;
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  s1 = cpu.eax;
  rtl_msb(&s0, &s1, 1); //msb of al
  rtl_msb(&s1, &s1, 2); //msb of ax
  if (decinfo.isa.is_operand_size_16) {
    rtl_sext(&cpu.eax,&cpu.eax,1);
  } //cwde
  else {
    rtl_sext(&cpu.eax,&cpu.eax,2);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
