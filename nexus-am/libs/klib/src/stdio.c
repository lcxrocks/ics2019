#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
  for (; *fmt; fmt++)
  {
    if( *fmt != '%' ){
      *out++ = *fmt;
      continue;
    }
    cnt++;
    fmt++; //skip '%'
    switch(*fmt){
      case 's': strcat(out,va_arg(ap,char *));
                continue;

      case 'd': *out++ = va_arg(ap,int);
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

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
