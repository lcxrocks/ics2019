#include "nemu.h"

paddr_t page_translate(vaddr addr, bool wren){
  PDE pde;
  PTE pte;
  if (cpu.cr0.paging)
  {
    uint32_t dir = addr >> 22; // 10 bits of DIR 页目录索引
    uint32_t page_idx = (addr >> 12) & 0x3ff; //10 bits of PAGE 页表索引
    uint32_t offset = addr & PAGE_MASK; // 12 bits of PAGE offset 页内地址
    
    paddr_t page_dir = dir *4 + cpu.cr3.page_directory_base << 12;
    pde.val = paddr_read(page_dir, 4); //get the page_directory 页表基地址
    assert(pde.present); //if pde.present == 0, then the page is not usable
    pde.accessed = 1; // “由MMU在进行地址转换时将该位置1”

    paddr_t page_addr = page_idx *4 + pde.page_frame << 12;
    pte.val = paddr_read(page_addr, 4);
    assert(pte.present);
    assert(!((!pte.read_write)&wren)) //can't write when (R/W = 0 && wren = 1)
    pte.accessed = 1;
    pte.dirty = wren ? 1 : pte.dirty; // “由MMU在进行写操作的时候将该位置1”

    return  ((pte.page_frame <<12) | offset)
  }
  else 
    return addr;
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if(((addr+len-1)& ~PAGE_MASK) != (addr & ~PAGE_MASK)) //addr + len cross page.end
  {
    printf("data cross the page boundary --> isa_vaddr_read()\n");
    assert(0);
  }
  else 
  {
    paddr_t paddr = page_translate(addr, false);
    return paddr_read(paddr,len);
  }  
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(((addr+len-1)& ~PAGE_MASK) != (addr & ~PAGE_MASK)) //addr +len cross page.end
  {
    printf("data cross the page boundary --> isa_vaddr_write()\n");
    assert(0);
  }
  else 
  {
    paddr_t paddr = page_translate(addr, true);
    paddr_write(paddr,len);
  }
}

