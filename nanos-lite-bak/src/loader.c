#include "proc.h"
#include <elf.h>
#include "fs.h"

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
size_t get_ramdisk_size();
extern char dispinfo[128];
static uintptr_t loader(PCB *pcb, const char *filename) { //
  Log("Load filename: %s\n",filename);
  Elf_Ehdr ehdr;
  int fd = fs_open(filename, 0, 0);
  //printf("openfile:%d\n",fd);
  fs_read(fd, &ehdr, sizeof(ehdr));
  //ramdisk_read(&ehdr, 0, sizeof(ehdr));
  Elf_Phdr phdr[ehdr.e_phnum]; //segement view
  fs_lseek(fd,ehdr.e_phoff,SEEK_SET);
  //printf("hhahahah\n");
  fs_read(fd, &phdr, ehdr.e_phentsize * ehdr.e_phnum);

  //ramdisk_read(&phdr, ehdr.e_phoff, ehdr.e_phentsize * ehdr.e_phnum);
  printf("hahhaah\n");
  for (uint16_t i = 0; i < ehdr.e_phnum; i++)
  {
    Log("Starting iteration: %d / %d\n",i+1,ehdr.e_phnum);
    if (phdr[i].p_type == PT_LOAD)
    {
    //   if(strlen(dispinfo)!=21){
    // printf("dispinfo changed to :%s\n",dispinfo);
    // assert(0);
  //}
      //Log("phdr[%d] p_type\n",i);
      size_t content[phdr[i].p_filesz];
      //ramdisk_read(content, phdr[i].p_offset, phdr[i].p_filesz);
      //Log("content size: %d\n",phdr[i].p_filesz);
      fs_lseek(fd, phdr[i].p_offset, SEEK_SET);
    //   if(strlen(dispinfo)!=21){
    // printf("dispinfo changed to :%s\n",dispinfo);
    // assert(0);
  //}
      //Log("lseek offset: %d\n",phdr[i].p_offset);
      //Log("fd: %d, contentsize:%d, phdrsize:%d\n",fd, strlen((char *)content), phdr[i].p_filesz);
      fs_read(fd, content, phdr[i].p_filesz);
      //Log("finished fs_read on line 54\n");
  //     if(strlen(dispinfo)!=21){
  //   printf("dispinfo changed to :%s\n",dispinfo);
  //   assert(0);
  // }
      //Log("read size: %d\n",phdr[i].p_filesz);
      uint32_t *p_start = (uint32_t *)phdr[i].p_vaddr;
      //fs_write(fd, p_start, phdr[i].p_filesz);
     // Log("Start load phdr[%d]\n",i);
      memcpy(p_start, content, phdr[i].p_filesz);
  //     if(strlen(dispinfo)!=21){
  //   printf("dispinfo changed to :%s\n",dispinfo);
  //   assert(0);
  // }
      //Log("Finished load phdr[%d], p_start:%x, content_length:%d, p_filesz:%d\n",i,p_start, sizeof(content), phdr[i].p_filesz);
      if (phdr[i].p_memsz > phdr[i].p_filesz) //.bss
      {
        char *bss_start = (char *)(phdr[i].p_vaddr + phdr[i].p_filesz);
        //Log("bss_start:%x\n",*bss_start);
        memset(bss_start, 0, phdr[i].p_memsz - phdr[i].p_filesz);
        //Log("Finished memset\n");
      }
    }
    //Log("finished iteration :%d /%d\n", i+1, ehdr.e_phnum);
  }
  //fs_close(fd);
  //Log("Finished Load\n");
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