#include "nb_instr.h"

static void jsr(unsigned short* a)
{
    memory[--SP] = PC;
    PC = *a;
    cycles += 3;
}

static void NONE(unsigned short* a)
{}

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