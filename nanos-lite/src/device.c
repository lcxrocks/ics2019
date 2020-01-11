#include "common.h"
#include <amdev.h>
// int32_t SCREEN_W;
// int32_t SCREEN_H;
extern int choose_pcb;
size_t serial_write(const void *buf, size_t offset, size_t len) {
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
  int cnt = 0;
  int key = read_key();
  char tmp[1024]={};
  bool down =false;
  if(key&8000) 
  {
    down = true;
    switch(key&0x7fff)
    {
      case _KEY_F1: choose_pcb = 1; break;
      case _KEY_F2: choose_pcb = 2; break;
      case _KEY_F3: choose_pcb = 3; break;
      default: Log("should not reach events_read() end \n"); assert(0); break;
    }
  }
  //printf("key : %x\n",key);
  if((key&0x7fff)!=_KEY_NONE){
    if(down) cnt = sprintf(tmp, "kd %s\n", keyname[key & 0x7fff]);
    else cnt = sprintf(tmp, "ku %s\n", keyname[key & 0x7fff]);
  }
  else 
    cnt = sprintf(tmp, "t %d\n", uptime());
  if (len < cnt)
    cnt = len;
  memcpy(buf, tmp, len);
  return cnt;
}

//static 
char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  // if(strlen(dispinfo)!=21){
  //   printf("dispinfo changed to :%s\n",dispinfo);
  //   assert(0);
  // }
  int cnt;
  int size = strlen(dispinfo);
  //Log("buf:%s ---offset:%d---len:%d---dispinfosize:%d\n",buf, offset,len,size);
  if(len + offset > size) cnt = size-offset;
  else cnt = len; 
  memcpy(buf, (&dispinfo)+offset, cnt); 
  return cnt;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  uint32_t pixel_addr= offset/4; //uint32_t *pixel;
  int SCREEN_W = screen_width();
  int x = pixel_addr % SCREEN_W;
  int y = pixel_addr / SCREEN_W;
  int cnt = len/4;
  for (int i = 0; i < cnt; i++)
  {
    draw_rect(&((uint32_t *)buf)[i], x, y, 1, 1);
    if(x==SCREEN_W)
    {
      x=0;
      y++;
    }
    else x++;
  }
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  int screen_w = screen_width();
  int screen_h = screen_height();
  //printf("screen_w: %d\n",screen_w);
  //printf("screen_h: %d\n",screen_h);
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_w,screen_h);
  Log("dispinfo:%slength = %d\n",dispinfo,strlen(dispinfo));
}
