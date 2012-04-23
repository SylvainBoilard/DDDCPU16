#include "adv.h"

#include <stdio.h>

static unsigned short* pop(void)
{
    return memory + SP++;
}

static unsigned short* peek(void)
{
    return memory + SP;
}

static unsigned short* push(void)
{
    return memory + --SP;
}

static unsigned short* sp(void)
{
    return &SP;
}

static unsigned short* pc(void)
{
    return &PC;
}

static unsigned short* o(void)
{
    return &O;
}

static unsigned short* nwad(void)
{
    ++cycles;
    return memory + memory[PC++];
}

static unsigned short* nw(void)
{
    static unsigned short value;
    value = memory[PC++];
    ++cycles;
    return &value;
}

unsigned short* (* const advanced[])(void) = {
    pop, peek, push, sp, pc, o, nwad, nw
};
