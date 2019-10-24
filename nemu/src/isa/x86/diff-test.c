#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool equal = memcmp(&cpu, ref_r, DIFFTEST_REG_SIZE);
  return !equal;
}

void isa_difftest_attach(void) {
}
