#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char _buf[256]={};
char str[256]={};
char output[1024]={};
char *itoa(int val, char *_buf, int base){
    memset(_buf,0,sizeof(_buf));
    memset(str,0,sizeof(str));
    char *p = _buf;
    unsigned digval;
    if(val < 0){
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }

    do{
        digval = (unsigned) (val % base);
        val /= base;

        if(digval > 9)
            *p++ = (char)(digval -10 +'a');
        else
            *p++ = (char)(digval + '0');

    }while(val > 0);
    *p++ = '\0';
    int len = strlen(_buf);
    for (int i = 0; i < len; i++)
    {
        str[i] = _buf[len-1-i]; 
    }

    return str;
}

int printf(const char *fmt, ...) {
  
  int ans;
  memset(output,0,sizeof(output));
  char *f=output;
  va_list ap;
  va_start(ap,fmt);
  ans = vsprintf(output, fmt, ap);
  va_end(ap);
  for(;*f;f++)
  {  _putc(*f);
    f++;
  }
  return ans;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
  char *s;
  int string_length=0;
  for (; *fmt; fmt++)
  {
    if( *fmt != '%' ){
      *out++ = *fmt;
      continue;
    }
    cnt++;
    fmt++; //skip '%'
    switch(*fmt){
      case 's': s = va_arg(ap, char *);
                out = strcpy(out, s);  
                string_length=strlen(s);
                out+=string_length;
                continue;

      case 'd': itoa(va_arg(ap, int),_buf,10);
                //printf("_buf1: %s\n",str);  
                out = strcpy(out, str);
                //printf("_buf: %s\n",str);  
                string_length=strlen(str);
                out+=string_length;
                continue;
    }
  }
  
  return cnt;
}

int sprintf(char *out, const char *fmt, ...) {
  int ans;
  va_list ap;
  memset(out,0,strlen(out));
  va_start(ap,fmt);
  ans = vsprintf(out,fmt,ap);
  va_end(ap);

  return ans;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
