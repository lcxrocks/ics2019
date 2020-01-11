#include "memory.h"
#include "proc.h"

static void *pf = NULL;
int _map(_AddressSpace *as, void *va, void *pa, int prot);
extern PCB *current;

void* new_page(size_t nr_page) {
  //panic("new_page!!");
  void *p = pf; //get the new page frame
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
/* 把新申请的堆区映射到虚拟地址空间中, 这样才能保证运行在分页机制上的用户进程可以正确地访问新申请的堆区. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  uintptr_t max_brk = current->max_brk;
  printf("max brk: %8x, brk: %8x\n",max_brk,brk);
  if (max_brk<brk)
  {
    void *va = NULL;
    void *pa = NULL;
    void *begin = (void *)PGROUNDUP(current->max_brk); //round off to next page
    void *end = (void *)PGROUNDUP(brk);
    for (void *i = begin; i < end; i+=PGSIZE)
    {
      va = (void*) i;
      pa = new_page(1);
      _map(&current->as,va,pa,0);
    }
    current->max_brk = brk;
  }
  while (max_brk<=brk)
  {
    void *pa = new_page(1);
    _map(&current->as, (void *)current->max_brk, pa, 0);
    current->max_brk += PGSIZE;
  }
  
  return 0;
}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start); //get a new page frame
  Log("free physical pages starting from %p", pf); 

  _vme_init(new_page, free_page);
}
