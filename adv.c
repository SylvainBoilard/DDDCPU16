#include "adv.h"

#include <stdio.h>

unsigned short* pop(void)
{
    return memory + SP++;
}

unsigned short* peek(void)
{
    return memory + SP;
}

unsigned short* push(void)
{
    return memory + --SP;
}

unsigned short* sp(void)
{
    return &SP;
}

unsigned short* pc(void)
{
    return &PC;
}

unsigned short* o(void)
{
    return &O;
}

unsigned short* nwad(void)
{
    return memory + memory[PC++];
}

unsigned short* nw(void)
{
    static unsigned short value;
    value = memory[PC++];
    return &value;
}

unsigned short* (* advanced[])(void) = {
    pop, peek, push, sp, pc, o, nwad, nw
};
