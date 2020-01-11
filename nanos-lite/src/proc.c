#include "proc.h"

#define MAX_NR_PROC 4
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
    context_uload(&pcb[1],"/bin/pal");
    //context_uload(&pcb[1],"/bin/pal");
    context_uload(&pcb[0],"/bin/hello");

    switch_boot_pcb();

    Log("Initializing processes...");

    //naive_uload(NULL, "/bin/pal");
}

_Context* schedule(_Context* prev)
{
    //Log("Schedule ... \n");
    current->cp = prev;
    current = &pcb[1]; //always select pcb[0] as the new process (for now)
    current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
    return current->cp;
}
