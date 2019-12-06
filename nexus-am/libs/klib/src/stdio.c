// #include "klib.h"
// #include <stdarg.h>

// #if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// char _buf[256]={};
// char str[256]={};
// char output[1024]={};
// char *myitoa(int val, char *_buf, int base){
//     memset(_buf,0,sizeof(_buf));
//     memset(str,0,sizeof(str));
//     char *p = _buf;
//     unsigned digval;
//     int neg = 0;
//     if(val < 0 && base == 10){
//         neg = 1;
//         val = (unsigned long)(-(long)val);
//     }

//     do{
//         digval = (unsigned) (val % base);
//         val /= base;

//         if(digval > 9)
//             *p++ = (char)(digval -10 +'a');
//         else
//             *p++ = (char)(digval + '0'); //didn't support hex number. Keep working.
//     }while(val > 0);
//     if(neg) *p++ = '-';
//     *p++ = '\0';
//     int len = strlen(_buf);
//     for (int i = 0; i < len; i++)
//     {
//         str[i] = _buf[len-1-i]; 
//     }

//     return str;
// }

// int printf(const char *fmt, ...) {
  
//   int ans;
//   memset(output,0,sizeof(output));
//   char *f=output;
//   va_list ap;
//   va_start(ap,fmt);
//   ans = vsprintf(output, fmt, ap);
//   va_end(ap);
//   for(;*f;f++)
//     _putc(*f);
//   return ans;
// }

// int vsprintf(char *out, const char *fmt, va_list ap) {
//   char *s;
//   char *start = out;
//   int string_length=0;
//   for (; *fmt; fmt++)
//   {
//     if( *fmt != '%' ){
//       *out++ = *fmt;
//       continue;
//     }
//     fmt++; //skip '%'
//     switch(*fmt){
//       case 's': s = va_arg(ap, char *);
//                 out = strcpy(out, s);  
//                 string_length=strlen(s);
//                 out+=string_length;
//                 continue;

//       case 'd': myitoa(va_arg(ap, int),_buf,10);
//                 //printf("_buf1: %s\n",str);  
//                 out = strcpy(out, str);
//                 //printf("_buf: %s\n",str);  
//                 string_length=strlen(str);
//                 out+=string_length;
//                 continue;
//       case 'x': myitoa(va_arg(ap,int),_buf,16);
//                 out = strcpy(out,str);
//                 string_length=strlen(str);
//                 out+=string_length;
//                 continue;
//     }
//   }
//   int cnt = out - start;
//   return cnt;
// }

// int sprintf(char *out, const char *fmt, ...) {
//   int ans;
//   va_list ap;
//   memset(out,0,strlen(out));
//   va_start(ap,fmt);
//   ans = vsprintf(out,fmt,ap);
//   va_end(ap);

//   return ans;
// }

// int snprintf(char *out, size_t n, const char *fmt, ...) {
//   return 0;
// }

// #endif
#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char *int2str (int value, int base) {  
  static char s[64];
  int i = sizeof(s) - 1;
  do {
    s[--i] = value % base + '0';  
    value /= base;
  } while(value > 0);
  return &s[i];
}

int printf(const char *fmt, ...) {
  static char buf[512];
  va_list ap;
  va_start(ap, fmt);
  int len = vsprintf(buf, fmt, ap);
  va_end(ap);
  size_t i;
  
  for (i = 0; buf[i] != '\0'; i++)
    _putc(buf[i]);

  return len;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int d;
  char c;
  char *str;
  size_t i = 0;

  while (*fmt) {
    if ('%' == *fmt) {
      fmt ++;
      switch (*fmt++) {
        case '%':
          out[i] = '%';
          i ++;
          break;
        case 's':
          str = va_arg(ap, char *);
          strncpy(&out[i], str, strlen(str));
          i += strlen(str);
          break;
        case 'c':
          c = va_arg(ap, int);
          out[i] = c;
          i ++;
          break;
        case 'd':
          d = va_arg(ap, int);
          str = int2str(d, 10);
          strncpy(&out[i], str, strlen(str));
          i += strlen(str);
          break;
      }
    }
    else {
      out[i] = *fmt;
      fmt ++;
      i ++;
    }
  }
  out[i] = '\0';
  va_end(ap);
  return i;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int len = vsprintf(out, fmt, ap);
  va_end(ap);
  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif