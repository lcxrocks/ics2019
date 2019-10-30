//#include "klib.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int vsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
  for (; *fmt; fmt++)
  {
    if( *fmt != '%' ){
      *out++ = *fmt;
      printf("this char: %c\n",*out);
      continue;
    }
    cnt++;
    fmt++; //skip '%'
    switch(*fmt){
      case 's': strcat(out,va_arg(ap,char *));
                continue;

      case 'd': 
                ;int fuckCcode = va_arg(ap,int);
                printf("fuckCcode : %d\n",fuckCcode);
                *out = fuckCcode;
                printf("out: %d\n", out);
                out++;
                continue;
    }
  }
  return cnt;
}

int sprintf(char *out, const char *fmt, ...) {
  int ans;
  va_list ap;

  va_start(ap,fmt);
  ans = vsprintf(out,fmt,ap);
  va_end(ap);

  return ans;
}

int main(){
    char buf[100];
    int a=300;
    char str[30]="I have an apple";
    sprintf(buf, "i have a pencil which cost me %d yuan", a);
    printf("buf: %s\n", buf);
}