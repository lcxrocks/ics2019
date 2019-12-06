#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define keydown_mask 0x8000
#define i8024_data_port 0x60
size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      kbd->keycode = inl(i8024_data_port);
      if(kbd->keycode&keydown_mask)kbd->keydown = 1;
			else kbd->keydown=0;
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
