#include "adv.h"

#include <stdio.h>

static unsigned short* popush(unsigned short is_a)
{
    /* This value has numerous problems, contact Notch */
    return memory + (is_a ? SP++ : --SP);
}

static unsigned short* peek(unsigned short is_a)
{
    return memory + SP;
}

static unsigned short* pick(unsigned short is_a)
{
    ++cycles;
    return memory + SP + memory[PC++];
}

static unsigned short* sp(unsigned short is_a)
{
    return &SP;
}

static unsigned short* pc(unsigned short is_a)
{
    return &PC;
}

static unsigned short* ex(unsigned short is_a)
{
    return &EX;
}

static unsigned short* nw(unsigned short is_a)
{
    ++cycles;
    return memory + memory[PC++];
}

static unsigned short* nwlit(unsigned short is_a)
{
    static unsigned short value;
    value = memory[PC++];
    ++cycles;
    return &value;
}

unsigned short* (* const advanced[])(unsigned short is_a) = {
    popush, peek, pick, sp, pc, ex, nw, nwlit
};
