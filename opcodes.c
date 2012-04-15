#include "opcodes.h"

#define RNW(op) (op & 0xf0 == 0x10) && (!(op & 0x08) || (op & 0x06 == 0x06))

void nbi(unsigned short* a, unsigned short* b)
{

}

void set(unsigned short* a, unsigned short* b)
{
    *a = *b;
}

void add(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a + *b;
    *a = result;
    O = result >> 16;
}

void sub(unsigned short* a, unsigned short* b)
{
    int result = *a - *b;
    *a = result;
    O = result >> 16;
}

void mul(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a * *b;
    *a = result;
    O = result>> 16;
}

void div(unsigned short* a, unsigned short* b)
{
    if (*b)
    {
	unsigned int result = (*a << 16) / *b;
	*a = result >> 16;
	O = result;
    }
    else
	*a = O = 0;
}

void mod(unsigned short* a, unsigned short* b)
{
    if (*b)
	*a %= *b;
    else
	*a = 0;
}

void shl(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a << *b;
    *a = result;
    O = result >> 16;
}

void shr(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a << (16 - *b);
    *a = result >> 16;
    O = result;
}

void and(unsigned short* a, unsigned short* b)
{
    *a &= *b;
}

void bor(unsigned short* a, unsigned short* b)
{
    *a |= *b;
}

void xor(unsigned short* a, unsigned short* b)
{
    *a ^= *b;
}

void ife(unsigned short* a, unsigned short* b)
{
    if (*a != *b)
	PC += RNW(memory[PC] >> 4 & 0x3F) + RNW(memory[PC] >> 10) + 1;
}

void ifn(unsigned short* a, unsigned short* b)
{
    if (*a == *b)
	PC += RNW(memory[PC] >> 4 & 0x3F) + RNW(memory[PC] >> 10) + 1;
}

void ifg(unsigned short* a, unsigned short* b)
{
    if (*a <= *b)
	PC += RNW(memory[PC] >> 4 & 0x3F) + RNW(memory[PC] >> 10) + 1;
}

void ifb(unsigned short* a, unsigned short* b)
{
    if (!(*a & *b))
	PC += RNW(memory[PC] >> 4 & 0x3F) + RNW(memory[PC] >> 10) + 1;
}

void (* opcodes[])(unsigned short* a, unsigned short* b) = {
    nbi, set, add, sub, mul, div, mod, shl, shr, and, bor, xor, ife, ifn, ifg, ifb
};
