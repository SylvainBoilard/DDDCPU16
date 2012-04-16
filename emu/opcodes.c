#include "opcodes.h"

/* This macro checks if the given value reads a word. */
#define VRW(op) (op & 0x30 == 0x10) && (!(op & 0x08) || (op & 0x06 == 0x06))

static void nbi(unsigned short* a, unsigned short* b)
{

}

static void set(unsigned short* a, unsigned short* b)
{
    *a = *b;
}

static void add(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a + *b;
    *a = result;
    O = result >> 16;
}

static void sub(unsigned short* a, unsigned short* b)
{
    int result = *a - *b;
    *a = result;
    O = result >> 16;
}

static void mul(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a * *b;
    *a = result;
    O = result>> 16;
}

static void div(unsigned short* a, unsigned short* b)
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

static void mod(unsigned short* a, unsigned short* b)
{
    if (*b)
	*a %= *b;
    else
	*a = 0;
}

static void shl(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a << *b;
    *a = result;
    O = result >> 16;
}

static void shr(unsigned short* a, unsigned short* b)
{
    unsigned int result = *a << (16 - *b);
    *a = result >> 16;
    O = result;
}

static void and(unsigned short* a, unsigned short* b)
{
    *a &= *b;
}

static void bor(unsigned short* a, unsigned short* b)
{
    *a |= *b;
}

static void xor(unsigned short* a, unsigned short* b)
{
    *a ^= *b;
}

static void ife(unsigned short* a, unsigned short* b)
{
    if (*a != *b)
	PC += VRW(memory[PC] >> 4 & 0x3F) + VRW(memory[PC] >> 10) + 1;
}

static void ifn(unsigned short* a, unsigned short* b)
{
    if (*a == *b)
	PC += VRW(memory[PC] >> 4 & 0x3F) + VRW(memory[PC] >> 10) + 1;
}

static void ifg(unsigned short* a, unsigned short* b)
{
    if (*a <= *b)
	PC += VRW(memory[PC] >> 4 & 0x3F) + VRW(memory[PC] >> 10) + 1;
}

static void ifb(unsigned short* a, unsigned short* b)
{
    if (!(*a & *b))
	PC += VRW(memory[PC] >> 4 & 0x3F) + VRW(memory[PC] >> 10) + 1;
}

void (* opcodes[])(unsigned short* a, unsigned short* b) = {
    nbi, set, add, sub, mul, div, mod, shl, shr, and, bor, xor, ife, ifn, ifg, ifb
};
