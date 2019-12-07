#ifndef __CPU_DECODE_H__
#define __CPU_DECODE_H__

#include "common.h"

#define make_DHelper(name) void concat(decode_, name) (vaddr_t *pc)
typedef void (*DHelper) (vaddr_t *);

#define OP_STR_SIZE 40
enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };

typedef struct {
  uint32_t type;
  int width;
  union {
    uint32_t reg;
    rtlreg_t addr; //rtl can only use this 
    uint32_t imm;
    int32_t simm;
  }; // why union? becuse this is the source : reg/addr/imm/simm. what is simm?
  rtlreg_t val; //rtl can only use this 
  char str[OP_STR_SIZE];
} Operand;

#include "isa/decode.h"

typedef struct {
  uint32_t opcode;
  uint32_t width;
  vaddr_t seq_pc;  // sequential pc
  bool is_jmp;
  vaddr_t jmp_pc;
  Operand src, dest, src2;
  struct ISADecodeInfo isa;
} DecodeInfo;

void operand_write(Operand *, rtlreg_t *);

/* shared by all helper functions */
extern DecodeInfo decinfo;

#define id_src (&decinfo.src) //for convience. now i can use `id_src` to represent `&decinfo.src`
#define id_src2 (&decinfo.src2)
#define id_dest (&decinfo.dest)

#ifdef DEBUG
#define print_Dop(...) snprintf(__VA_ARGS__)
#else
#define print_Dop(...)
#endif

#endif
