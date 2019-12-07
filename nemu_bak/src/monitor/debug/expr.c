#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
uint32_t isa_reg_str2val(char *, bool *);
enum {
  TK_NOTYPE = 256, TK_EQ,  NUM, REGS, HEX,TK_NEQ, TK_ANDL,TK_DECONF,TK_ORL
  /* TODO: Add more token types */
 //So the initial value for space is 256, incrementing 1 each element afterwards.
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},       //not-equal
  {"/", '/'},           // divide
  {"\\*", '*'},           // multiply or DECONP
  {"\\-", '-'},           //minus
  {"[&][&]", TK_ANDL},           //logic-and
  {"[|][|]", TK_ORL},            //logic-or
  {"\\(", '('},           //left parenthesis
  {"\\)", ')'},           //right parenthesis
  {"0x[0-9,a-f,A-F]+", HEX},             //hex-numbers*/
  {"[0-9]+", NUM},             //numbers
  {"[$][a-z]+", REGS},          //regs
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )
//number of regex
static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[1024] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  memset(tokens,'\0',sizeof(tokens));
  nr_token = 0;
  while (e[position] != '\0' && e[position] != '\n') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position; // pos=substr_start=pmatch.so
        int substr_len = pmatch.rm_eo;

       // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
         //   i, rules[i].regex, position, substr_len, substr_len, substr_start);
           //pmatch.so:regex's start;
           //pmatch.eo:regex's end;        
        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        //printf("rules[%d].token_type: %d\n", i, rules[i].token_type);
        switch (rules[i].token_type) {
          case TK_NOTYPE: break; //do nothing
          case '+':
          case '-':
          case '*':
          case '/':
          case '(':
          case ')':
          case TK_NEQ: 
          case TK_EQ:
          case TK_ANDL:
          case TK_ORL:
                    tokens[nr_token].type=rules[i].token_type;
                    nr_token++;
                    break; 
          case NUM:
          
          case HEX: tokens[nr_token].type=rules[i].token_type;
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    nr_token++;
                    break;
          case REGS:tokens[nr_token].type=rules[i].token_type;
                    strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);
                    nr_token++;
                    break; //skip '$'
          
          default: tokens[nr_token].type=rules[i].token_type;
                    strncpy(tokens[nr_token].str,substr_start,substr_len);
                    nr_token++;
                    break;
        }
    
        break;
      }
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p, int q){
int left=0;
if(tokens[p].type=='(' && tokens[q].type==')')   
 {       
    for(int i=p; i<=q; i++){
            if(tokens[i].type=='(') left++;
            if(tokens[i].type==')') left--;
            if(left==0 && i!=q) return false; //found right par in the middle;
        }
    if(left==0) return true;
    else return false;
    }
   // printf("checking parentheses %d, %d\n", p ,q );
    
    else return false;
    /********
    if(tokens[p].type=='(' && tokens[q].type==')') return true;
    else return false;
    //The simplest version of checking parentheses****/
 }

int eval(int p, int q){
    //printf("calling eval(%d, %d)\n", p, q);
    if(p>q) {
        printf("Left boundary exceeds the right one? What are you even thinking?(p(%d)>q(%d))\n",p,q);
        assert(0);
    }
    else if(p==q){
        char *stop;
        bool success=true;
        if(tokens[p].type==NUM) 
            return atoi(tokens[p].str); 
        else if(tokens[p].type==HEX) 
            return strtoul(tokens[p].str,&stop, 16);
        else if(tokens[p].type==REGS)
            return isa_reg_str2val(tokens[p].str, &success);
        else{
        printf("I don't think this expr stands for anything.(p(%d)=q(%d))\n",p,q);
        assert(0);
        }
    }
    else if(check_parentheses(p,q)) {
      //printf("calling inside parentheses eval(%d,%d)\n", p+1,q-1); 
      return eval(p+1,q-1);
      }
    else{
        int op=-1;
        int leftp=0;
        bool pm=false; //plus or minus
        for(int i=p; i<=q; i++){
            if(tokens[i].type=='(') leftp++;
            if(tokens[i].type==')') leftp--;
            if(leftp==0){
                if(tokens[i].type=='+'||tokens[i].type=='-') {op=i;pm=true;} 
                else if(((tokens[i].type=='*'||tokens[i].type=='/'))&&(pm==false)) op=i;
                else if(tokens[i].type==TK_EQ || tokens[i].type==TK_NEQ || tokens[i].type==TK_ORL || tokens[i].type==TK_ANDL) op=i;
            }
        }
        //printf("op: %d\n",op);
        int val1=0;
        int val2=0;
        if(op){
        //printf("calling forward eval(%d,%d)\n",p,op-1);
          val1=eval(p,op-1);
        //printf("calling backward eval(%d,%d)\n",op+1,q);
          val2=eval(op+1,q);
        }
        switch(tokens[op].type){
            case '+': return val1+val2;
            case '-': return val1-val2;
            case '*': 
                      if(op==p){
                          char *end;
                          vaddr_t addr=strtol(tokens[op+1].str, &end, 16);
                          return vaddr_read(addr,4);
                      }
                      else
                          return val1*val2;

            case '/': return val1/val2;
            case TK_ORL: return val1 || val2;
            case TK_ANDL: return val1 && val2;
            case TK_EQ: return val1==val2;
            case TK_NEQ: return val1!=val2;
            default : printf("switch glitch at line : %d",__LINE__); assert(0);
            //Simplest version of eval
        }
    }
}
uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  //printf("nr_token is :%d\n",nr_token);
  /* TODO: Insert codes to evaluate the expression. */
  uint32_t nrtoken=nr_token-1; 
  //printf("nrtoken is %d\n",nrtoken);
  return  eval(0,nrtoken);
}
