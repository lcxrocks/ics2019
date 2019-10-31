/*
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *mystrcpy(char* dst,const char* src) {
  size_t i=0;
  while(src[i]!='\0'){
    dst[i]=src[i];
    i++;
  }
  dst[i]='\0';
  return dst;
}
size_t mystrlen(const char *s) {
  size_t cnt=0;
  while(s[cnt]!='\0') cnt++;
  //_putc(cnt);
  return cnt;
}

void* mymemset(void* v,int c,size_t n) {
  int i;
  for (i = 0; i < n; i++){
    ((char*)v)[i] = (unsigned char)c;
  }
  return v;
}

char buf[256]={};
char str[256]={};
char *myitoa(int val, char *buf, int base){
    mymemset(buf,0,sizeof(buf));
    mymemset(str,0,sizeof(str));
    char *p = buf;
    char *firstdig = str;
    char tmp;
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
    int len = mystrlen(buf);
    for (int i = 0; i < len; i++)
    {
        str[i] = buf[len-1-i]; 
    }

    return str;
}

int myvsprintf(char *out, const char *fmt, va_list ap) {
  int cnt = 0;
  char *s;
  int string_length=0;
  for (; *fmt; fmt++)
  {
    if( *fmt != '%' ){
      *out++ = *fmt;
      //printf("this char: %c\n",*(out-1));
      continue;
    }
    cnt++;
    fmt++; //skip '%'
    switch(*fmt){
      case 's': ;
                s = va_arg(ap, char *);
                out = mystrcpy(out, s);  
                string_length=mystrlen(s);
                out+=string_length;
                continue;

      case 'd': 
                myitoa(va_arg(ap, int),buf,10);
                printf("buf1: %s\n",str);  
                out = mystrcpy(out, str);
                printf("buf: %s\n",str);  
                string_length=mystrlen(str);
                out+=string_length;
                continue;
    }
  }
  return cnt;
}

int mysprintf(char *out, const char *fmt, ...) {
  int ans;
  va_list ap;

  va_start(ap,fmt);
  ans = myvsprintf(out,fmt,ap);
  va_end(ap);

  return ans;
}

int main(){
    char buf[100];
    int a=300;
    char str[30]="I have an apple";
    mysprintf(buf, "i have a pencil which cost me %d yuan, %s", a, str);
    printf("buf: %s\n", buf);
}*/