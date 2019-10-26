#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include "rtl/rtl.h"

/* RTL pseudo instructions */

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(dest, &reg_l(r)); return;
    case 1: rtl_host_lm(dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline void rtl_sr(int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(&reg_l(r), src1); return;
    case 1: rtl_host_sm(&reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(&reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline void rtl_push(const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
  cpu.esp -= 4; //stack increment
  rtl_sm(&cpu.esp, src1, 4); //write to esp

}

static inline void rtl_pop(rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
  rtl_lm(dest, &cpu.esp, 4);
  cpu.esp += 4;
}

static inline void rtl_is_sub_overflow(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 - src2)
  rtl_msb(&t0, src1, width);
  rtl_msb(&t1, src2, width);
  rtl_msb(dest, res, width);
  if((t0!=t1)&&(t0!= *dest)) *dest=1;
  else *dest=0;
  //TODO();
}
// add width. 
static inline void rtl_is_sub_carry(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 - src2)
  /*
  int mask=(1<<(width*8))-1;
  t0 = *res & mask;
  t1 = *src1 & mask;*/
  
  if(*res > *src1) *dest=1;
  else *dest=0;
  
}

static inline void rtl_is_add_overflow(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 + src2)
  rtl_msb(&t0, src1, width);
  rtl_msb(&t1, src2, width);
  rtl_msb(dest, res, width);
  if((t0==t1)&&(t0!= *dest)) *dest=1;
  else *dest=0;
  //TODO();
}

static inline void rtl_is_add_carry(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 + src2)
  /*
  int mask=(1<<(width*8))-1;
  t0 = *res & mask;
  t1 = *src1 & mask;*/
  //printf("res: %8x\n", *res);
  //printf("src: %8x\n", *src1);
  if( *res < *src1 ) *dest=1;
  else *dest=0;
  //printf("ressssssssssssss: %8x\n", *dest);
  //TODO();
}

#define make_rtl_setget_eflags(f) \
  static inline void concat(rtl_set_, f) (const rtlreg_t* src) { \
    cpu.f=*src; \
  } \
  static inline void concat(rtl_get_, f) (rtlreg_t* dest) { \
    *dest=cpu.f; \
  }// by lcx

make_rtl_setget_eflags(CF)
make_rtl_setget_eflags(OF)
make_rtl_setget_eflags(ZF)
make_rtl_setget_eflags(SF)

static inline void rtl_update_ZF(const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
  t0=(4-width)*8;//remove the redundant bits out of our sight
  cpu.ZF=(((*result & (0xFFFFFFFF)>>t0))== 0 );
}

static inline void rtl_update_SF(const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  t0=width*8-1;//lower-bits
  //printf("cpu.SF=%8x",cpu.SF);
  cpu.SF=((*result >> t0) & 1);
  //printf("cpu.SF=%8x",cpu.SF);
}

static inline void rtl_update_ZFSF(const rtlreg_t* result, int width) {
  rtl_update_ZF(result, width);
  rtl_update_SF(result, width);
}

#endif
