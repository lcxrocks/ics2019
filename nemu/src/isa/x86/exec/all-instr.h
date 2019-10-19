#include "cpu/exec.h"
//arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(cmp);
//cc.c


//control.c
make_EHelper(call);
make_EHelper(ret);
//data-mov.c
make_EHelper(push);
make_EHelper(mov);
make_EHelper(lea);
//logic.c
make_EHelper(xor);
make_EHelper(and);
//prefix.c

//special.c
make_EHelper(operand_size);
make_EHelper(inv);
make_EHelper(nop);
//system.c
make_EHelper(nemu_trap);
