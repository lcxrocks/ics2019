#include "proc.h"
#include <elf.h>
int fs_open(const char *pathname);
size_t fs_read(int fd, void *buf, size_t len);

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
  Elf_Ehdr ehdr;
  int fd = fs_open(filename);
  fs_read(fd, &ehdr, sizeof(ehdr));
  //ramdisk_read(&ehdr, 0, sizeof(ehdr));
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
