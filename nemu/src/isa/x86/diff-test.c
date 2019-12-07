#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool equal = memcmp(&cpu, ref_r, DIFFTEST_REG_SIZE);

  printf("eax_QEMU 0x%8x %8u\n", ref_r->eax,ref_r->eax);
  printf("ecx_QEMU 0x%8x %8u\n", ref_r->ecx, ref_r->ecx);
  printf("edx_QEMU 0x%8x %8u\n", ref_r->edx,ref_r->edx);
  printf("ebx_QEMU 0x%8x %8u\n", ref_r->ebx,ref_r->ebx);
  printf("esp_QEMU 0x%8x %8u\n", ref_r->esp,ref_r->esp);
  printf("ebp_QEMU 0x%8x %8u\n", ref_r->ebp,ref_r->ebp);
  printf("esi_QEMU 0x%8x %8u\n", ref_r->esi,ref_r->esi);
  printf("edi_QEMU 0x%8x %8u\n", ref_r->edi,ref_r->edi);
  printf("eip_QEMU 0x%8x %8u\n", ref_r->pc,ref_r->pc);
  printf("############################################\n");
//rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
  assert(equal == true);
  return !equal;
}

void isa_difftest_attach(void) {
}
