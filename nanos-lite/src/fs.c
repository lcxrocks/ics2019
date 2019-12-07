#include "fs.h"

size_t serial_write(const void* buf, size_t offset, size_t len);
size_t ramdisk_read(void* buf, size_t offset, size_t len);
size_t ramdisk_write(const void* buf, size_t offset, size_t len);
size_t events_read(void* buf, size_t offset, size_t len);
size_t fbsync_write(const void* buf, size_t offset, size_t len);
size_t dispinfo_read(void* buf, size_t offset, size_t len);
size_t fb_write(const void* buf, size_t offset, size_t len);
int screen_width();
int screen_height();

typedef size_t (*ReadFn)(void* buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void* buf, size_t offset, size_t len);

typedef struct {
    char* name;
    size_t size;
    size_t disk_offset;
    ReadFn read;
    WriteFn write;
    size_t open_offset;
} Finfo;

enum { FD_STDIN,
       FD_STDOUT,
       FD_STDERR,
       FD_FB };

size_t invalid_read(void* buf, size_t offset, size_t len)
{
    panic("should not reach here");
    return 0;
}

size_t invalid_write(const void* buf, size_t offset, size_t len)
{
    panic("should not reach here");
    return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
    {"stdin", 0, 0, invalid_read, invalid_write},
    {"stdout", 0, 0, invalid_read, serial_write},
    {"stderr", 0, 0, invalid_read, serial_write},
    {"/dev/fb", 0, 0, invalid_read, fb_write},
    {"/dev/events", 0, 0, events_read, invalid_write},
    {"/dev/fbsync", 0, 0, invalid_read, fbsync_write},
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
  //Log("file_table finished int!(size: %d)",file_table[FD_FB].size);
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
    //printf("str comparing for the %d time\n", i);
  }
  panic("File not found!\n");
}

int fs_close(int fd)
{
    return 0;
}

size_t fs_read(int fd, void* buf, size_t len)
{
    Finfo* cur_file = &file_table[fd];
    if (!cur_file->read) {
        if (cur_file->open_offset > cur_file->size) return 0;
        if (len > cur_file->size - cur_file->open_offset)
            len = cur_file->size - cur_file->open_offset;
        ramdisk_read(buf, cur_file->disk_offset + cur_file->open_offset, len);
    } else
        len = cur_file->read(buf, cur_file->disk_offset + cur_file->open_offset, len);
    cur_file->open_offset += len;
    return len;
}

size_t fs_write(int fd, const void* buf, size_t len)
{
    Finfo* cur_file = &file_table[fd];
    if (!cur_file->write) {
        if (cur_file->open_offset > cur_file->size) return 0;
        if (len > cur_file->size - cur_file->open_offset)
            len = cur_file->size - cur_file->open_offset;
        ramdisk_write(buf, cur_file->disk_offset + cur_file->open_offset, len);
    } else
        len = cur_file->write(buf, cur_file->disk_offset + cur_file->open_offset, len);
    cur_file->open_offset += len;
    return len;
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