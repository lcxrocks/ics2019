#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) 
{
  for(int i=0;i<len;i++)
	{
		_putc(*((char *)buf+i));
	}
	return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) 
{
  int Key=read_key();
	if(Key!=_KEY_NONE)
	{
		if(Key&0x8000)len=sprintf(buf,"kd %s\n",keyname[Key&0xfff]);
		else len=sprintf(buf,"ku %s\n",keyname[Key&0xfff]);
	}
	else 
	{
		len=sprintf(buf,"t %d\n",uptime());
		//printf("the hahahahhahahahhaha is %010d\n",uptime());
	}
  
  return len;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) 
{
	if(offset+len>strlen(dispinfo))len=strlen(dispinfo)-offset;
	memcpy(buf,&dispinfo[offset],len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) 
{
  int w=screen_width(),x=(offset>>2)%w,y=(offset>>2)/w,tot=len>>2;
	draw_rect((uint32_t *)buf,x,y,tot,1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) 
{
	draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
	//printf("the dispinfo is %s\n",dispinfo);
}
