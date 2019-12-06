// #include "common.h"
// #include <amdev.h>
// // int32_t SCREEN_W;
// // int32_t SCREEN_H;
// size_t serial_write(const void *buf, size_t offset, size_t len) {
//   //printf("that buf: %s\n",buf);
//   for (int i = 0; i < len; i++)
//     _putc(((const char*)buf)[i]);
//   return len;
// }

// #define NAME(key) [_KEY_##key] = #key,

// static const char *keyname[256] __attribute__((used)) = {
//   [_KEY_NONE] = "NONE",
//   _KEYS(NAME)
// };

// size_t events_read(void *buf, size_t offset, size_t len) {
//   //read event into buf;
//   //printf("events read!!!!!\n");
//   int cnt = 0;
//   int key = read_key();
//   char tmp[1024]={};
//   bool down =false;
//   if(key&8000) down = true;
//   //printf("key : %x\n",key);
//   if((key&0x7fff)!=_KEY_NONE){
//     if(down) cnt = sprintf(tmp, "kd %s\n", keyname[key & 0x7fff]);
//     else cnt = sprintf(tmp, "ku %s\n", keyname[key & 0x7fff]);
//   }
//   else 
//     cnt = sprintf(tmp, "t %d\n", uptime());
//   if (len < cnt)
//     cnt = len;
//   memcpy(buf, tmp, len);
//   return cnt;
// }

// static char dispinfo[128] __attribute__((used)) = {};

// size_t dispinfo_read(void *buf, size_t offset, size_t len) {
//   int cnt;
//   int size = strlen(dispinfo);
//   Log("buf:%s ---offset:%d---len:%d---dispinfosize:%d\n",buf, offset,len,size);
//   if(len + offset > size) cnt = size-offset;
//   else cnt = len; 
//   memcpy(buf, (&dispinfo)+offset, cnt);
//   return cnt;
// }

// // size_t dispinfo_read(void *buf, size_t offset, size_t len) {
// //   strncpy(buf, dispinfo + offset, len);
// //   Log("buf:%s --- offset:%d ---len:%d ---dispinfosize:%d\n",buf,offset,len);
// //   return len;
// // }

// size_t fb_write(const void *buf, size_t offset, size_t len) {
//   uint32_t pixel_addr= offset/4; //uint32_t *pixel;
//   int SCREEN_W = screen_width();
//   int x = pixel_addr % SCREEN_W;
//   int y = pixel_addr / SCREEN_W;
//   int cnt = len/4;
//   for (int i = 0; i < cnt; i++)
//   {
//     draw_rect(&((uint32_t *)buf)[i], x, y, 1, 1);
//     if(x==SCREEN_W)
//     {
//       x=0;
//       y++;
//     }
//     else x++;
//   }
//   return len;
// }

// size_t fbsync_write(const void *buf, size_t offset, size_t len) {
//   draw_sync();
//   return len;
// }

// void init_device() {
//   Log("Initializing devices...");
//   _ioe_init();
//   int screen_w = screen_width();
//   int screen_h = screen_height();
//   printf("screen_w: %d\n",screen_w);
//   printf("screen_h: %d\n",screen_h);
//   sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_w,screen_h);
//   Log("dispinfo:%s\n",dispinfo);
//   // TODO: print the string to array `dispinfo` with the format
//   // described in the Navy-apps convention
// }
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

