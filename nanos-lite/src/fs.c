#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB}; //0,1,2,3 fd value;

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write}, //占位
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname)
{
  for (size_t i = 2; i < NR_FILES; i++)
  {
    if(strcmp(file_table[i].name, pathname)==0)
      return i;
  }
  printf("File not found!\n");
  assert(0);
}

int fs_close(int fd)
{
  return 0;
}

size_t fs_read(int fd, void *buf, size_t count)
{
  int size = file_table[fd].size; 
  int disk_offset = file_table[fd].offset;
  if(count > size)
  {
    printf("fs_read() too much!\n");
    return -1;
  }
  else
  {
    memcpy(buf,&ramdisk_start+dis_offset,count);
    return count;
  }
  
}