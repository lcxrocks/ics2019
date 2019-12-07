#include <amtest.h>
#include <amdev.h>

#define NAME(key) \
  [_KEY_##key] = #key,

const char *names[256] = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

void keyboard_test() {
  printf("Try to press any key...\n");
  while (1) {
    //printf("Entering this iteration\n");
    int key = read_key();
    //printf("key is %d\n",key);
    int down = 0;
    if (key & 0x8000) { //key &0x(1000)000 !=0 -> key =0x1.*;
      key ^= 0x8000; //cut the first bit;
      down = 1; //pressed
    }
    if (key != _KEY_NONE) {
      printf("Get key: %d %s %s\n", key, names[key], down ? "down" : "up");
    }
  }
}
