#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void isa_reg_display();
uint32_t expr();
uint32_t vaddr_read();
WP* new_wp();
void free_wp();
void watchpoint_info();
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_si(char *args){
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  if (arg==NULL) cpu_exec(1);
  else{
     printf("arg: %s\n",arg);
     int steps = atoi(arg);
     cpu_exec(steps);
}    
   //printf("done");
   return 0;
}

static int cmd_info (char *args){
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  if(strcmp(arg, "r")==0) isa_reg_display();
  else if(strcmp(arg, "w")==0) watchpoint_info();
  else 
  {
    printf("Invaild operation");
    return -1;
  }
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_x(char *args) {
 /* extract the first argument */
  char *arg = strtok(NULL, " ");
  if(arg==NULL) {
  printf("Please tell me how many times I should excute.");
  return -1;
  }
  int n = atoi(arg);
  
  char *EXPR = strtok(NULL, " ");
  if(EXPR==NULL){
  printf("Show me the location please");
  return -1;
  }
  if(strtok(NULL, " ")){
  printf("Too many parameters!");
  return -1;
  }

  char *str;
  vaddr_t addr=strtol(EXPR, &str, 16); //convert to hex
  for(int i=0; i<n; i++){
    uint32_t data=vaddr_read(addr+4*i,4);
    printf("0x%08x :", addr+4*i);
    for(int j=0; j<4; j++){
        printf("0x%02x  ", data &0xff);
        data>>=8;
    }
    printf("\n");
  }
  return 0;
}

static int cmd_help(char *args);
static int cmd_p(char *args); 
static int cmd_w(char *args);
static int cmd_d(char *args);
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "single-step execution N times", cmd_si},
  { "info", "'info r' print out the value of all register/'info w' watchpoint", cmd_info},
  { "x", "print out the value(s) of given address", cmd_x},
  { "p", "print expr's val and dereference and print the register", cmd_p},
  { "w", "set watchpoint at expr", cmd_w},
  { "d", "delete watchpoint [i]", cmd_d},
  /* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_w(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg==NULL){
    printf("Give me any expr to watch plz.\n");
    assert(0);
  }
  WP* wp=new_wp();
  bool success=true;
  sscanf(arg, "%s", wp->expr);
  wp->old_value = expr(args, &success);
  printf("Watchpoint[%d] Successfully set at expr: %s with its original val: %d\n", wp->NO,wp->expr, wp->old_value);
  return 0;
}

static int cmd_d(char *args){
  //d 1 or d 1-2 or d all
  char *arg = strtok(NULL, " ");
  int num = atoi(arg);
  free_wp(num);
  printf("Deleted watchpoint %d\n",num);
  return 0;
}
static int cmd_p(char *args) {
   bool lcxnb=true;
   char *arg = strtok(NULL, " ");
   uint32_t expr_cpt=expr(arg, &lcxnb);
   printf("%u    %8x\n",expr_cpt,expr_cpt);
   return 0;
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
