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
size_t fs_offset(int fd);
size_t fs_openoffset(int fd);
/*
  _Context *cp;
  _AddressSpace as; (size_t pgsize; //页面的大小
                      _Area area; //start & end 虚拟地址空间中用户态的范围
                      void *ptr;//指示具体的映射？)
  uintptr_t max_brk;
*/
#define DEFAULT_ENTRY ((void *)0x40000000); 
// static uintptr_t loader(PCB *pcb, const char *filename) { //
//   int fd = fs_open(filename, 0, 0);
//   Elf_Ehdr ehdr;
//   Elf_Phdr phdr;
//   Log("Load filename: %s\n",filename);
//   fs_read(fd, &ehdr, sizeof(ehdr));
//   for (uint16_t i = 0; i < ehdr.e_phnum; i++)
//   {
//     fs_lseek(fd,ehdr.e_phoff+ i*ehdr.e_phentsize,SEEK_SET);
//     fs_read(fd, &phdr, ehdr.e_phentsize);
//     if (phdr.p_type == PT_LOAD)
//     {
//       fs_lseek(fd, phdr.p_offset, SEEK_SET);
//       void *pa = DEFAULT_ENTRY;
//       void *va = DEFAULT_ENTRY;
//       va = (void *)phdr.p_vaddr;
//       int pg_num=0;
//       for (size_t j = 0; j < phdr.p_memsz; j+=PGSIZE)
//       {
//         size_t bytes = ((phdr.p_memsz-i)>=PGSIZE) ? PGSIZE : (phdr.p_memsz-i);
//         pa = new_page(1);
//         pg_num++;
//         _map(&pcb->as,va,pa,0);
//         fs_read(fd,pa,bytes);
//         pcb->max_brk = (uintptr_t)va+PGSIZE;
//         va+=PGSIZE;
//       }
//       memset((void *)pa-(pg_num-1)*PGSIZE+phdr.p_filesz,0,phdr.p_memsz - phdr.p_filesz);
//     }
//     //Log("finished iteration :%d /%d\n", i+1, ehdr.e_phnum);
//   }
//   fs_close(fd);
//   Log("Finished Load\n");
//   return ehdr.e_entry;
// }
static uintptr_t loader(PCB *pcb, const char *filename) {
  //printf("enter loader\n");
  Elf_Ehdr elfheader;
  Elf_Phdr programheader;
  int fd = fs_open(filename,0,0);
  if(fd!=-1){
    //printf("before read\n");
    fs_read(fd,&elfheader,sizeof(Elf_Ehdr));
    //printf("%x\n",elfheader.e_entry);
    fs_lseek(fd,elfheader.e_phoff,SEEK_SET);
    for(uint16_t i=0;i<elfheader.e_phnum;i++){
      fs_read(fd,&programheader,sizeof(Elf_Phdr));
      size_t opset=fs_openoffset(fd);
      if(programheader.p_type == PT_LOAD){
        fs_lseek(fd,programheader.p_offset,SEEK_SET);
        void *vaddr=NULL, *paddr=NULL;
        vaddr = (void*)programheader.p_vaddr;
        int count=0;
        //printf("%x\n",programheader.p_vaddr);
        //printf("%x\n",vaddr);
        for(size_t i=0,sz = programheader.p_memsz;i<sz;i+=PGSIZE){
          size_t read_bytes = ((sz-i)>=PGSIZE) ? PGSIZE : (sz-i);
          //printf("%x\n",read_bytes);
          paddr = new_page(1);
          count++;
          //printf("%x\n",paddr);
          //printf("%x  %x\n",vaddr,paddr);
          _map(&pcb->as,vaddr,paddr,0);
          fs_read(fd,paddr,read_bytes);
          pcb->max_brk = (uintptr_t)vaddr+PGSIZE;
          vaddr+=PGSIZE;
          //memset((void*)paddr+programheader.p_filesz,0,(programheader.p_memsz-programheader.p_filesz));
        }
        memset((void*)paddr-(count-1)*PGSIZE+programheader.p_filesz,0,(programheader.p_memsz-programheader.p_filesz));

      }
      fs_lseek(fd,opset,SEEK_SET);
    }
  }
  fs_close(fd);
  //TODO();
  //printf("going out loader\n");
  return elfheader.e_entry;
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
  _protect(&pcb->as);
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void*)entry, NULL);
}
