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
  
  if(decinfo.isa.is_operand_size_16){
    
    printf("pusha 16 bit unimplemented!\n");
    TODO();
    /*rtl_lm(&s0,&cpu.gpr[R_SP]_16,2);
    rtl_push(&cpu.gpr[R_AX]._16);
    rtl_push(&cpu.gpr[R_CX]._16);
    rtl_push(&cpu.gpr[R_DX]._16);
    rtl_push(&cpu.gpr[R_BX]._16);
    rtl_push(&s0);
    rtl_push(&cpu.gpr[R_BP]._16);
    rtl_push(&cpu.gpr[R_SI]._16);
    rtl_push(&cpu.gpr[R_DI]._16);*/
  }
  else
  {
    rtl_lm(&s0,&cpu.esp,4);
    rtl_push(&cpu.eax);
    rtl_push(&cpu.ecx);
    rtl_push(&cpu.edx);
    rtl_push(&cpu.ebx);
    rtl_push(&s0);
    rtl_push(&cpu.ebp);
    rtl_push(&cpu.esi);
    rtl_push(&cpu.edi);
  }
  
  print_asm("pusha");
}

make_EHelper(popa) {
  if(decinfo.isa.is_operand_size_16){
    printf("popa 16 bit unimplemented!\n");
    TODO();
  }
  else
  {
    rtl_pop(&cpu.edi);
    rtl_pop(&cpu.esi);
    rtl_pop(&cpu.ebp);
    rtl_pop(&cpu.ebx);//throw away esp;
    rtl_pop(&cpu.ebx);
    rtl_pop(&cpu.edx);
    rtl_pop(&cpu.ecx);
    rtl_pop(&cpu.eax);
  }

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
  /*
  s1 = cpu.eax;
  rtl_msb(&s0, &s1, 1); //msb of al
  rtl_msb(&s1, &s1, 2); //msb of ax
  */
  if (decinfo.isa.is_operand_size_16) {
    rtl_sext(&s0,&cpu.eax,1);
    cpu.gpr[R_AX]._16 = s0;
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

make_EHelper(movs){
  if(id_dest->width != 1)
    id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  s0 = cpu.esi;
  rtl_lm(&s1, &s0, id_dest->width);
  s0 = cpu.edi;
  rtl_sm(&s0, &s1, id_dest->width);
  
  print_asm_template2(movs);
}