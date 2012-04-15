#include "values.h"

unsigned short* reg(unsigned short v)
{
    return registers + (v & 0x0007);
}

unsigned short* regad(unsigned short v)
{
    return memory + registers[v & 0x0007];
}

unsigned short* nwreg(unsigned short v)
{
    return memory + ((memory[PC++] + registers[v & 0x0007]) & 0xFFFF);
}

unsigned short* adv(unsigned short v)
{
    return advanced[v & 0x0007]();
}

unsigned short* lit(unsigned short v)
{
    static unsigned short value;
    value = v & 0x001F;
    return &value;
}

unsigned short*(* values[])(unsigned short v) = {
    reg, regad, nwreg, adv, lit, lit, lit, lit
};
