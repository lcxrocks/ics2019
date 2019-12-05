#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <cstdio>

int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  printf("NDL_LOADBitMap succes\n");
  assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  printf("OpenDisplay success\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  printf("DrawRect success\n");
  NDL_Render();
  printf("NDL_Render success\n");
  NDL_CloseDisplay();
  printf("ND_Close success\n");
  while (1);
  return 0;
}
