#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool equal = memcmp(&cpu, ref_r, DIFFTEST_REG_SIZE);
  
  printf("eax_diff 0x%8x %8u\n", ref_r->eax,ref_r->eax);
  printf("ecx 0x%8x %8u\n", ref_r->ecx, ref_r->ecx);
  printf("edx 0x%8x %8u\n", ref_r->edx,ref_r->edx);
  printf("ebx 0x%8x %8u\n", ref_r->ebx,ref_r->ebx);
  printf("esp 0x%8x %8u\n", ref_r->esp,ref_r->esp);
  printf("ebp 0x%8x %8u\n", ref_r->ebp,ref_r->ebp);
  printf("esi 0x%8x %8u\n", ref_r->esi,ref_r->esi);
  printf("edi 0x%8x %8u\n", ref_r->edi,ref_r->edi);
  printf("############################################\n");
//rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
  
  return !equal;
}

void isa_difftest_attach(void) {
}
