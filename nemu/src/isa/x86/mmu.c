#include "nemu.h"
// 从x86.h里抄过来的
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/\------ OFF(va) ------/
#define PDX(va)     (((uint32_t)(va) >> 22) & 0x3ff)
#define PTX(va)     (((uint32_t)(va) >> 12) & 0x3ff)
#define OFF(va)     ((uint32_t)(va) & 0xfff)
#define PTE_ADDR(pte)   ((uint32_t)(pte) & ~0xfff)
paddr_t page_translate(vaddr_t addr, bool wren) {
    //aka page_walk
    PDE pde, *pgdir;
    PTE pte, *pgtab;
    // 只有进入保护模式并开启分页机制后才会进行页级地址转换。。。。。。。。。。
    if (cpu.cr0.paging) {
	    pgdir = (PDE *)(PTE_ADDR(cpu.cr3.val)); //cr3存放20位的基址作为页目录入口
	    pde.val = paddr_read((paddr_t)&pgdir[PDX(addr)], 4);
	    assert(pde.present);
	    pde.accessed = 1;

	    pgtab = (PTE *)(PTE_ADDR(pde.val));  //页目录存放20位的基址作为页表入口
	    pte.val = paddr_read((paddr_t)&pgtab[PTX(addr)], 4);
	    assert(pte.present);
	    pte.accessed = 1;
	    pte.dirty = wren ? 1 : pte.dirty; //写则置脏位

	    //pte高20位和线性地址低12位拼接成真实地址
	    return PTE_ADDR(pte.val) | OFF(addr); 
	}

    return addr;
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if(((addr+len-1)& ~PAGE_MASK) != (addr & ~PAGE_MASK)) //addr + len cross page.end
  {
    printf("data cross the page boundary --> isa_vaddr_read()\n");
    vaddr_t addr2 = (addr+len-1)& ~PAGE_MASK; //the start of next
    int pg1_len = addr2 - addr; //next page's start - addr = len_already_read
    int pg2_len = len - pg1_len; //what's left;
    uint32_t data;
    paddr_t paddr1 = page_translate(addr, false);
    paddr_t paddr2 = page_translate(addr2,false);
    data = paddr_read(paddr1, pg1_len) & (paddr_read(paddr2,pg2_len) << pg1_len);
    return data;
    //assert(0);
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
    paddr_write(paddr,data,len);
  }
}