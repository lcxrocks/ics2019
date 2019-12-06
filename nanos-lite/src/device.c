#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  int i = 0;
  char *ptr = (char *)buf;
  for (; i < len; i++)
    _putc(ptr[i]);
  return len;
  //return 0;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  if (key == _KEY_NONE) 
  {
    size_t val = sprintf((char *)buf, "t %d\n", uptime());
    //printf("%d %s\n", val, buf);
    return val;
  }
  else
  {
    char status = key & 0x8000 ? 'd' : 'u';
    return sprintf((char *)buf, "k%c %s\n", status, keyname[key & 0x7fff]);
  }
	//return 0;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  size_t count = (len + offset) > strlen(dispinfo) ? strlen(dispinfo) - offset : len;
  memcpy(buf, &(dispinfo[offset]), count);
  return strlen(&(dispinfo[offset]));
  //return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int width = screen_width();
  int y = (offset / 4) / width;
  int x = (offset / 4) % width;
  for (int i = 0; i < (len / 4); i++)
  {
    draw_rect(&((uint32_t *)buf)[i], x, y, 1, 1);
    if (x == width)
    {
      x = 0; 
      y++;
    }
    else
    {
      x++;
    }
    }
   return len;
//	return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
	//return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  int width = screen_width();
  int height = screen_height();
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", width,height);
}
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention

