#ifndef __FS_H__
#define __FS_H__

#include "common.h"

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif
int fs_open(const char *pathname);
int fs_close(int fd);
int fs_read(int fd, void *buf, size_t len);
int fs_write(int fd, void *buf, size_t len);
int fs_lseek(int fd, size_t offset, int whence);
#endif
