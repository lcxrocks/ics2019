#include "cpu/exec.h"
//arith.c
make_EHelper(sub);
make_EHelper(add);
make_EHelper(cmp);
//cc.c
make_EHelper(setcc);
//control.c
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
//data-mov.c
make_EHelper(push);
make_EHelper(mov);
make_EHelper(lea);
make_EHelper(movzx);
//logic.c
make_EHelper(test);
make_EHelper(xor);
make_EHelper(and);
//prefix.c

//special.c
make_EHelper(operand_size);
make_EHelper(inv);
make_EHelper(nop);
//system.c
make_EHelper(nemu_trap);
