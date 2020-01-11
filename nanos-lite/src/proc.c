#include "proc.h"

#define MAX_NR_PROC 4
int choose_pcb = 0;
void naive_uload(PCB* pcb, const char* filename);
void context_kload(PCB* pcb, void* entry);
void context_uload(PCB* pcb, const char* filename);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {}; //max process num = 4
static PCB pcb_boot = {};
PCB* current = NULL;

void switch_boot_pcb()
{
    current = &pcb_boot;
}

void hello_fun(void* arg)
{
    int j = 1;
    while (1) {
        Log("Hello World from Nanos-lite for the %dth time!", j);
        j++;
        _yield();
    }
}

void init_proc()
{
    context_uload(&pcb[0],"/bin/pal");
    //context_uload(&pcb[1],"/bin/pal");
    context_uload(&pcb[1],"/bin/pal");
    context_uload(&pcb[2],"/bin/hello");
    context_uload(&pcb[3],"/bin/hello");
    switch_boot_pcb();

    Log("Initializing processes...");

    //naive_uload(NULL, "/bin/pal");
}

int cnt=0;
_Context* schedule(_Context* prev)
{
    //Log("Schedule ... \n");
    current->cp = prev;
    current = &pcb[0]; //always select pcb[0] as the new process (for now)
    //current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
    if (cnt<0x400)
    {
        cnt++;
        printf("cnt:%x\n",cnt);
    }
    
    if (cnt == 0x400)
    {   
        cnt = 0;
        choose_pcb = 3;
    }
    
    
    switch (choose_pcb)
    {
    case 0:
        current = &pcb[0];
        break;
    case 1:
        Log("Switch to pcb[1]...\n");
        current = &pcb[1];
        break;
    case 2:
        Log("Switch to pcb[1]...\n");
        current = &pcb[2];
        break;
    case 3: 
        Log("Switch to pcb[1]...\n");
        current = &pcb[3];
        break;
    default:
        Log("Should not reach schedule() end. \n");
        assert(0);
        break;
    }
    return current->cp;
}
