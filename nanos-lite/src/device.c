#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //printf("that buf: %s\n",buf);
  for (int i = 0; i < len; i++)
    _putc(((const char*)buf)[i]);
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  //read event into buf;
  //printf("events read!!!!!\n");
  int cnt = 0;
  int key = read_key();
  char tmp[1024]={};
  bool down =false;
  if(key&8000) down = true;
  //printf("key : %x\n",key);
  if((key&0x7fff)!=_KEY_NONE){
    if(down) cnt = sprintf(tmp, "kd %s\n", keyname[key & 0x7fff]);
    else cnt = sprintf(tmp, "ku %s\n", keyname[key & 0x7fff]);
  }
  else {
    int time = uptime();
    cnt = sprintf(tmp, "t %d\n", uptime());
    Log("uptime : %d\n",time);
  }
  if (len < cnt)
    cnt = len;
  memcpy(buf, tmp, len);
  return cnt;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
