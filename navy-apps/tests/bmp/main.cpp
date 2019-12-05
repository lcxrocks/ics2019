#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
//#include <cstdio>

int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  cout<<"NDL_LOADBitMap succes"<<endl;
  assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  cout<<"OpenDisplay success"<<endl;
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  cout<<"DrawRect success"<<endl;
  NDL_Render();
  cout<<"NDL_Render success"<<endl;
  NDL_CloseDisplay();
  cout<<"ND_Close success"<<endl;
  while (1);
  return 0;
}
