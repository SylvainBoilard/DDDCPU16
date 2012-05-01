#include "values.h"

static unsigned short* reg(unsigned short v, unsigned short is_a)
{
    return registers + (v & 0x0007);
}

static unsigned short* regad(unsigned short v, unsigned short is_a)
{
    return memory + registers[v & 0x0007];
}

static unsigned short* nwreg(unsigned short v, unsigned short is_a)
{
    ++cycles;
    return memory + (unsigned short)(memory[PC++] + registers[v & 0x0007]);
}

static unsigned short* adv(unsigned short v, unsigned short is_a)
{
    return advanced[v & 0x0007](is_a);
}

static unsigned short* lit(unsigned short v, unsigned short is_a)
{
    static unsigned short value;
    if (v == 0x003F)
	value = 0xFFFF;
    else
	value = v & 0x001F;
    return &value;
}

unsigned short*(* const values[])(unsigned short v, unsigned short is_a) = {
    reg, regad, nwreg, adv, lit, lit, lit, lit
};
