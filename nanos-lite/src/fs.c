#include "fs.h"
size_t events_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fbsync_write(const void *buf, size_t offset, size_t len);
//extern uint8_t ramdisk_start;
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len) ;
size_t serial_write(const void *buf, size_t offset, size_t len) ;
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
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/fb",0, 0, invalid_read, fb_write},
  {"/dev/events" ,0, 0, events_read, invalid_write}, 
  {"/dev/fbsync", 0, 0,invalid_read, fbsync_write},
  {"/proc/dispinfo", 128, 0, dispinfo_read, invalid_write},
  
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int W = screen_width();
  int H = screen_height();
  file_table[FD_FB].size = W*H*4;
  file_table[FD_FB].open_offset = 0;
  Log("file_table finished int!(size: %d)",file_table[FD_FB].size);
}

int fs_open(const char *pathname, int flags, int mode)
{
  Log("pathname :%s\n",pathname);
  for (size_t i = 0; i < NR_FILES; i++)
  {
    if(strcmp(file_table[i].name, pathname)==0)
    { 
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("File not found!\n");
}
// 
int fs_close(int fd)
{
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len)
{
  //Log("---fd: %d ---buf:%s; --- len:%d\n",fd, buf,len);
  int size = file_table[fd].size; 
  int disk_offset = file_table[fd].disk_offset;
  int open_offset = file_table[fd].open_offset;
  size_t read_start = disk_offset + open_offset;  
  if(open_offset + len > size)
      len = size - open_offset;
  size_t ret=0;
  Log("fs_read: fd: %d, len:%d\n",fd, len);
  if(!file_table[fd].read){
    Log("ret: %d\n",ret);
    ret = ramdisk_read(buf, read_start,len);
    Log("ret: %d\n",ret);
  }
  else{
    Log("retelse: %d\n",ret);
    ret = file_table[fd].read(buf, read_start, len);
    Log("retelse: %d\n",ret);
  }
  file_table[fd].open_offset += ret;
  //printf("ret: %d\n",ret); 
  return ret;
}

size_t fs_lseek(int fd, size_t offset, int whence)
{
  switch (whence)
  {
  case SEEK_SET:
    file_table[fd].open_offset = offset;
    break;
  case SEEK_CUR:
    file_table[fd].open_offset += offset;
    break;
  case SEEK_END:
    file_table[fd].open_offset = file_table[fd].size + offset;
    break;
  default: panic("Should not reach fs_lseek() end\n");
    break;
  }
  return file_table[fd].open_offset;
}

size_t fs_write(int fd, const void *buf, size_t len){
    int size = file_table[fd].size; 
    int disk_offset = file_table[fd].disk_offset;
    int open_offset = file_table[fd].open_offset;
    size_t write_start = disk_offset + open_offset;
    size_t ram_write_len = len;
    if(open_offset + len >= size)
      ram_write_len = size - open_offset;
    int ret;
    if(!file_table[fd].write)
      ret = ramdisk_write(buf, write_start,ram_write_len);
    else 
      ret = file_table[fd].write(buf, write_start, len);
    file_table[fd].open_offset +=ret;
    return ret; //VFS
}