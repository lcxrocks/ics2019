#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
/* read `len' bytes starting from `offset' of ramdisk into `buf' */
size_t ramdisk_read(void *buf, size_t offset, size_t len);
/* write `len' bytes starting from `buf' into the `offset' of ramdisk */
size_t ramdisk_write(const void *buf, size_t offset, size_t len) ;
/*return the size of ramdisk. unit: 1 byte*/
size_t get_ramdisk_size() ;

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO(); WRONG!!!!
  // ramdisk_read((void *)0x3001000,0x1000,0x3a43);
  // ramdisk_read((void *)0x3005000,0x5000,0x1918);
  // ramdisk_read((void *)0x3008000,0x7000,0x8d8);
  // return ((uintptr_t)0x3000000);
  Elf_Ehdr ehdr;
  ramdisk_read(&ehdr, 0, sizeof(ehdr));
//   typedef struct
// {
//   Elf32_Word	p_type;			/* Segment type */
//   Elf32_Off	p_offset;		/* Segment file offset */
//   Elf32_Addr	p_vaddr;		/* Segment virtual address */
//   Elf32_Addr	p_paddr;		/* Segment physical address */
//   Elf32_Word	p_filesz;		/* Segment size in file */
//   Elf32_Word	p_memsz;		/* Segment size in memory */
//   Elf32_Word	p_flags;		/* Segment flags */
//   Elf32_Word	p_align;		/* Segment alignment */
// } Elf32_Phdr;
  Elf_Phdr phdr[ehdr.e_phnum]; //segement view
  ramdisk_read(&phdr, ehdr.e_phoff, ehdr.e_phentsize * ehdr.e_phnum);

  for (uint16_t i = 0; i < ehdr.e_phnum; i++)
  {
    if (phdr[i].p_type == PT_LOAD)
    {
      size_t content[phdr[i].p_filesz];
      ramdisk_read(content, phdr[i].p_offset, phdr[i].p_filesz);
      uint32_t *ptr1 = (uint32_t *)phdr[i].p_vaddr;
      memcpy(ptr1, content, phdr[i].p_filesz);

      if (phdr[i].p_memsz > phdr[i].p_filesz) //.bss
      {
        char *ptr2 = (char *)(phdr[i].p_vaddr + phdr[i].p_filesz);
        memset(ptr2, 0, phdr[i].p_memsz - phdr[i].p_filesz);
      }
    }
  }
return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
