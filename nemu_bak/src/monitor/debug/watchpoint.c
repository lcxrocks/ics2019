#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32
//32 watchpoints at most 
static WP wp_pool[NR_WP] = {}; //init an array of WP
static WP *head = NULL, *free_ = NULL; //head ptr and free ptr, boths points to certain WP

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i; //enumerate the watchpoints
    wp_pool[i].next = &wp_pool[i + 1]; //this watchpoint points to the next
    strcpy(wp_pool[i].expr,"");
    wp_pool[i].old_value = -1; 
  }
  wp_pool[NR_WP - 1].next = NULL; // the last points to NULL

  head = NULL; //now head is a null ptr
  free_ = wp_pool; //free points to the first WP, meaning that the whole pool is free
}

WP* search(WP* next_ptr){
    WP* tmp=head; //start from head
    if(next_ptr==head) return NULL;
    while (tmp->next!=next_ptr && tmp->next!=NULL && tmp!= NULL)
    {
      printf("searching wp_pool[%d]\n",tmp->NO);
      tmp=tmp->next;
    }
    printf("return val: wp_pool[%d]\n",tmp->NO);
    return tmp;
} //search the linked list, next_ptr:tmp->next; ex.search(NULL) returns the last ptr in the linked list

WP* new_wp(){
  WP* new_wp_ptr=free_;

  if(free_==NULL)
  {  
      printf("Watchpoint full\n");
      assert(0); 
  }
  else if (head==NULL){
    printf("assigning head\n");
    head = new_wp_ptr;//wp_pool[0]
    free_= free_->next;
    new_wp_ptr->next=NULL;
    return new_wp_ptr;
  }
  else 
  { 
    WP* last=search(NULL);
    last->next=new_wp_ptr; 
    free_=free_->next;
    new_wp_ptr->next=NULL;
    return new_wp_ptr;
  }
}

void free_wp(int num){
  if (head==NULL)
  {
    printf("No watchpoint available\n");
    assert(0);
  }
  else{
    /***** won't work. because the watch point NO's sequnce is not monototic.
    WP* last=search(NULL);
    if(num>last->NO)
    {
      printf("(num) %d > (last watch point) %d", num, last->NO);
      assert(0);
    }
    else 
    { ***/
        WP* wp=&wp_pool[num]; 
        strcpy(wp->expr,"\0");
        wp->old_value=-1;
        WP* ptr_before_wp=search(wp);
        if(ptr_before_wp==NULL){
          head=wp->next;
          wp->next=free_;
          free_=wp;
        }
        else{
          ptr_before_wp->next=wp->next;
          wp->next=free_;
          free_=wp;
        }
    //}
}
}


bool check_watchpoint(){
  WP* tmp=head;
  uint32_t val=0;
  bool success=true;
  if(tmp==NULL)
      return true;
  else 
  while (tmp!=NULL )
  {
      printf("tmp.expr: %s\n", tmp->expr);
      val=expr(tmp->expr,&success);
      
      if(val!=tmp->old_value) {
          printf("Watch point [%d] changed: old value is [%d], new value is [%d]\n",tmp->NO,tmp->old_value,val);
          //store old_val and expr will be implemented in ui.c
          tmp->old_value=val;
          return false; //means something has changed.
      }
      printf("Watch point [%d] didn't change: old value is [%d], new value is [%d]\n",tmp->NO,tmp->old_value,val);
      if(tmp->next==NULL)
        return true;
      else
        tmp=tmp->next;
  }
  return true; //means nothing changed 
  }

void watchpoint_info(){
    WP* tmp = head;
    printf("Watchpoint no.  |   expr  |  old_val \n");
    while(tmp!=NULL)
    {
        printf("[%d]            | %s    | %d (0x%8x)\n",tmp->NO, tmp->expr, tmp->old_value, tmp->old_value);
        tmp=tmp->next;
    }
}
/* TODO: Implement the functionality of watchpoint */


