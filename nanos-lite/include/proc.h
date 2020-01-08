#ifndef __PROC_H__
#define __PROC_H__

#include "common.h"
#include "memory.h"

#define STACK_SIZE (8 * PGSIZE) //2^3 * 2^12 = 2^15 = 32KB stack.

typedef union {
  uint8_t stack[STACK_SIZE] PG_ALIGN;
  struct {
    _Context *cp; //context pointer
    _AddressSpace as; 
    // we do not free memory, so use `max_brk' to determine when to call _map()
    uintptr_t max_brk;
  };
} PCB;

extern PCB *current;

#endif
