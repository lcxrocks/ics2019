#include "fs.h"
//extern uint8_t ramdisk_start;
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
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
    {  
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("File not found!\n");
}

int fs_close(int fd)
{
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len)
{
  int size = file_table[fd].size; 
  int disk_offset = file_table[fd].disk_offset;
  int open_offset = file_table[fd].open_offset;
  size_t read_start = disk_offset + open_offset;
  size_t read_end = open_offset + len;
  if(open_offset + len > size)
  {
    len = size - open_offset;
    read_end = size; 
  }
  int ret = ramdisk_read(buf, read_start, len);
  file_table[fd].open_offset = read_end;
  return ret;
}