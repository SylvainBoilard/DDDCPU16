#include "nb_instr.h"

static void jsr(unsigned short* a)
{
    memory[--SP] = PC;
    PC = *a;
}

static void NONE(unsigned short* a)
{
    ++a; /* Prevents unneeded warning. */
}

void (* const nb_instr[])(unsigned short* a) = {
    (void*)0, jsr, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};
