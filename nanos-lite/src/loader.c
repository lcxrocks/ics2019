#include "proc.h"
#include <elf.h>
#include <stdio.h>
#ifdef __ISA_AM_NATIVE__
#    define Elf_Ehdr Elf64_Ehdr
#    define Elf_Phdr Elf64_Phdr
#else
#    define Elf_Ehdr Elf32_Ehdr
#    define Elf_Phdr Elf32_Phdr
#endif

extern int fs_open(const char* pathname, int flags, int mode);
extern int fs_close(int fd);
extern size_t fs_read(int fd, void* buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
size_t get_file_size(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) { //
  Log("Load filename: %s\n",filename);
  Elf_Ehdr ehdr;
  Elf_Phdr phdr;
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &ehdr, sizeof(ehdr));
  for (uint16_t i = 0; i < ehdr.e_phnum; i++)
  {
    fs_lseek(fd,ehdr.e_phoff+ i*ehdr.e_phentsize,SEEK_SET);
    fs_read(fd, &phdr, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD)
    {
      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      uint32_t *p_start = (uint32_t *)phdr.p_vaddr;
      fs_read(fd, p_start,phdr.p_filesz);
      memset(p_start+phdr.p_filesz,0,phdr.p_memsz - phdr.p_filesz);
    }
    //Log("finished iteration :%d /%d\n", i+1, ehdr.e_phnum);
  }
  fs_close(fd);
  Log("Finished Load\n");
  return ehdr.e_entry;
}

void naive_uload(PCB* pcb, const char* filename)
{
    uintptr_t entry;
    entry = loader(pcb, filename);
    Log("Jump to entry = %x", entry);
    ((void (*)())entry)();
}

void context_kload(PCB* pcb, void* entry)
{
    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);

    pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB* pcb, const char* filename)
{
    uintptr_t entry = loader(pcb, filename);

    _Area stack;
    stack.start = pcb->stack;
    stack.end = stack.start + sizeof(pcb->stack);

    pcb->cp = _ucontext(&pcb->as, stack, stack, (void*)entry, NULL);
}
