#include <am.h>
#include <x86.h>
#include <nemu.h>
#include <string.h>
#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN = {};
static PTE kptabs[(PMEM_SIZE + MMIO_SIZE) / PGSIZE] PG_ALIGN = {};
static void* (*pgalloc_usr)(size_t) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static _Area segments[] = {      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE},
  {.start = (void*)MMIO_BASE,  .end = (void*)(MMIO_BASE + MMIO_SIZE)}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))

int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) {
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) {
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) {
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) {
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);
  vme_enable = 1;

  return 0;
}

int _protect(_AddressSpace *as) {
  PDE *updir = (PDE*)(pgalloc_usr(1));
  as->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) {
    updir[i] = kpdirs[i];
  }

  return 0;
}

void _unprotect(_AddressSpace *as) {
}

static _AddressSpace *cur_as = NULL;
void __am_get_cur_as(_Context *c) {
  c->as = cur_as;
}

void __am_switch(_Context *c) {
  if (vme_enable) {
    set_cr3(c->as->ptr);
    cur_as = c->as;
  }
}

int _map(_AddressSpace *as, void *va, void *pa, int prot) {
  //return 0; //用于将地址空间as中虚拟地址va所在的虚拟页, 以prot的权限映射到pa所在的物理页
  //当prot中的present位为0时，表示让va的映射无效
  uint32_t pdx = PDX(va); // 10 bits of DIR 页目录索引
  uint32_t ptx = PTX(va); //10 bits of PAGE 页表索引
  PDE *pgdir = (PDE *) as->ptr; //页目录基址
  PTE *ptdir; //页表基址
  PDE pde;
  PTE pte;
  pde = pgdir[pdx]; 
  if (!(pde & PTE_P)) //映射新的页表
  {
    PDE new_page_table = (uint32_t) pgalloc_usr(1) | PTE_P;
    pgdir[pdx] = new_page_table;
  }
  pde = pgdir[pdx];
  ptdir = (PTE *)PTE_ADDR(pde);
  pte = ptdir[ptx];
  if (!(pte & PTE_P))
  {
    ptdir[ptx] = (uint32_t) pa | PTE_P;
  }
  return 0;
  //   uint32_t pdx = PDX(va);
  // uint32_t ptx = PTX(va);
  // PDE pde = ((PDE *)as->ptr)[pdx];
  // if((pde & PTE_P) == 0){
  //   PDE *pt = (PDE*)(pgalloc_usr(1));
  //   PDE new_pde = (uintptr_t)pt | PTE_P;
  //   ((PDE*)as->ptr)[pdx] = new_pde;
  // }
  // pde = ((PDE *)as->ptr)[pdx];
  // PTE *page_table = (PTE*)PTE_ADDR(pde);
  // if((page_table[ptx]&PTE_P)==0){
  //   //pa = pgalloc_usr(1);
  //   page_table[ptx] = (uint32_t)pa | PTE_P;
  // }
  // return 0;
}

_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack, void *entry, void *args) {
  void *u_end = ustack.end - sizeof(int)- sizeof(char*) - sizeof(char*); 
  //   sizeof(main_stack_frame) = sizeof(argc) - sizeof(argv) - sizeof(envp)
  _Context *uc = (_Context *)u_end -1;
  memset(uc,0,sizeof(_Context));
  uc->pc = (uintptr_t)entry;
  uc->cs = 0x8;
  uc->eflag = 0x200;
  uc->as = as;
  return uc;
}
