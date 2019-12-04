
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
void _putc(char ch) {
  putchar(ch);
}
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

char _buf[256]={};
char str[256]={};
char *myitoa(int val, char *_buf, int base){
    mymemset(_buf,0,sizeof(_buf));
    mymemset(str,0,sizeof(str));
    char *p = _buf;
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
    int len = mystrlen(_buf);
    for (int i = 0; i < len; i++)
    {
        str[i] = _buf[len-1-i]; 
    }

    return str;
}
char output[1024]={};
int myprintf(const char *fmt, ...) {
  int ans;
  memset(output,0,sizeof(output));
  char *f=output;
  va_list ap;
  va_start(ap,fmt);
  ans = vsprintf(output, fmt, ap);
  va_end(ap);
  //printf("%s\n",output);
  
  for(;*f;f++)
    _putc(*f);
    return ans;
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
                myitoa(va_arg(ap, int),_buf,10);
                //printf("_buf1: %s\n",str);  
                out = mystrcpy(out, str);
                //printf("_buf: %s\n",str);  
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
  mymemset(out,0,mystrlen(out));
  va_start(ap,fmt);
  ans = myvsprintf(out,fmt,ap);
  va_end(ap);

  return ans;
}
char buf[128];
int main(){

  myprintf("%s", "Hello world!\n");
  //printf("hahahhahahah\n");
  //printf("buf1: %s\n",buf);
	char s[128] = "FUCK!!!\n";
	mysprintf(buf, "receive event %s\n", s);
  myprintf("buf2: %s\n",buf);
	//assert(strcmp(buf, "1 + 1 = 2\n") == 0);

	//mysprintf(buf, "%d + %d = %d\n", 2, 10, 12);
	//myprintf("buf3: %s\n",buf);
  //assert(strcmp(buf, "2 + 10 = 12\n") == 0);

	return 0;
}