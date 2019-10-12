#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[64000];
int const len=3; //Generating 3-bit long num at most
int const Max_Depth=3;
enum{NUM,OP,LEFT,RIGHT};

static inline void gen(int type){
switch(type){
    case NUM: {
              int tot_num=rand()%len+1; //At most 3-bit long num(decimal)
             // int const long_enough_char_array=2*len;
              char num_bit[10]="";
              int r_a=rand()%1000+1;
              /*******
              num_bit[0]=rand()%9+'1';
              for(int i=1;i<=tot_num;i++){
                num_bit[i]=rand()%10+'0';
              }
              */
              sprintf(num_bit,"%d", r_a);
              num_bit[tot_num+1]='\0'; //necessary for applying strcat
              strcat(buf,num_bit);
              break;
              }
    case OP:  {
              int rand_op=rand()%4;
              switch(rand_op){
                 case 0: strcat(buf,"+"); break;
                 case 1: strcat(buf,"-"); break;
                 case 2: strcat(buf,"*"); break;
                 case 3: strcat(buf,"/"); break;
                 default: break;
              }
              break;
              }

    case LEFT:  strcat(buf,"("); break;

    case RIGHT: strcat(buf,")"); break;
    }
    return;
}

static inline void gen_rand_expr(int Depth) {
    if(Depth > Max_Depth) {gen(NUM); return;}
    else{
        int rand_expr=rand()%3;
        switch(rand_expr){
            case 0: gen(NUM);
                    break;
            case 1: gen(LEFT);
                    //printf("generating LEFT P, depth %d\n", Depth);
                    gen_rand_expr(Depth+1);
                    gen(RIGHT);
                    //printf("generating RIGHT P, depth %d\n", Depth);
                    break;
            case 2: gen_rand_expr(Depth+1);
                    gen(OP);
                    gen_rand_expr(Depth+1);
                    break;
        }        
    }
    return;
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr(0);

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
