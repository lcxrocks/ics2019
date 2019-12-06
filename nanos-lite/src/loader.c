#include "fs.h"
#include "proc.h"
#include <elf.h>
#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
//size_t ramdisk_read(void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
   // TODO();
//   void *buf=NULL;
//   int buf=0x00000000; 
//   ramdisk_read((void *)buf,0x000000,0x0497f);
//   vaddr_write(0x83000000, *buf, 0x0497f);
//   ramdisk_read((void *)buf,0x005000,0x00878);
//   vaddr_write(0x83005000, *buf, 0x00878);
//   paddr_write((0x83005000+0x0083c),0,60);
//    return buf;
/*   Elf_Ehdr elf;
  ramdisk_read(&elf, 0, sizeof(elf));

  Elf_Phdr segment[elf.e_phnum];
  ramdisk_read(&segment, elf.e_phoff, elf.e_phentsize * elf.e_phnum);

  for (uint16_t i = 0; i < elf.e_phnum; i++)
  {
    if (segment[i].p_type == PT_LOAD)
    {
      size_t content[segment[i].p_filesz];
      //printf("reading...\n");
      ramdisk_read(content, segment[i].p_offset, segment[i].p_filesz);
      //printf("%x\n %x\n", segment[i].p_offset, segment[i].p_filesz);
      //printf("writing...\n");
      //printf("%x\n", segment[i].p_vaddr);
      //ramdisk_write(content, segment[i].p_vaddr - RAM_START, segment[i].p_filesz);
      uint32_t *ptr1 = (uint32_t *)segment[i].p_vaddr;
      memcpy(ptr1, content, segment[i].p_filesz);

      if (segment[i].p_memsz > segment[i].p_filesz)
      {
        //printf("dealing with vacant...\n");
        //char vacant[segment[i].p_memsz - segment[i].p_filesz];
        //for (int k = 0; k < segment[i].p_memsz-segment[i].p_filesz; k++) vacant[k] = 0;
        char *ptr2 = (char *)(segment[i].p_vaddr + segment[i].p_filesz);
        //ramdisk_write(vacant, segment[i].p_vaddr + segment[i].p_filesz - RAM_START, segment[i].p_memsz-segment[i].p_filesz);
        //memcpy(ptr2, vacant, segment[i].p_memsz-segment[i].p_filesz);   
        memset(ptr2, 0, segment[i].p_memsz - segment[i].p_filesz);
      }
    }
  }
return elf.e_entry;*/
  int fd = fs_open(filename);
  Elf_Ehdr elf;
  fs_read(fd, &elf, sizeof(Elf_Ehdr));

  Elf_Phdr segment[elf.e_phnum];
  fs_lseek(fd, elf.e_phoff, SEEK_SET);
  fs_read(fd, &segment, elf.e_phnum * elf.e_phentsize);

  for (uint16_t i = 0; i < elf.e_phnum; i++)
  {
    if (segment[i].p_type == PT_LOAD)
    {
      size_t content[segment[i].p_filesz];
      fs_lseek(fd, segment[i].p_offset, SEEK_SET);
      fs_read(fd, content, segment[i].p_filesz);
      uint32_t *ptr1 = (uint32_t *)segment[i].p_vaddr;
      memcpy(ptr1, content, segment[i].p_filesz);

      if (segment[i].p_memsz > segment[i].p_filesz)
      {
        char *ptr2 = (char *)(segment[i].p_vaddr + segment[i].p_filesz);
        memset(ptr2, 0, segment[i].p_memsz - segment[i].p_filesz);
      }
    }
  }
  return elf.e_entry;
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
