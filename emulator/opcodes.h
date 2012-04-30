#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

#include "globals.h"

extern void (* const opcodes[])(unsigned short* b, unsigned short* a);

#endif /* OPCODES_H_INCLUDED */
