#include <stdio.h>

#include "globals.h"
#include "opcodes.h"
#include "nb_instr.h"
#include "values.h"

static unsigned int lil_end()
{
    const unsigned int test = 0x00000001;
    return *(const unsigned char *)&test;
}

int main(int argc, char* argv[])
{
    FILE* ram_img;

    if (argc == 1)
    {
	printf("You must specify a RAM image to load.\n");
	return 1;
    }

    ram_img = fopen(argv[1], "rb");
    if (!ram_img)
    {
	printf("Cannot open %s\n.", argv[1]);
	return 1;
    }
    fread(memory, 1, 0x20000, ram_img);
    fclose(ram_img);
    if (lil_end())
    {
	unsigned char* raw_mem = (unsigned char*)memory;
	unsigned char temp;
	unsigned int i;
	for (i = 0; i < 0x20000; i += 2)
	{
	    temp = raw_mem[i];
	    raw_mem[i] = raw_mem[i + 1];
	    raw_mem[i + 1] = temp;
	}
    }

    while (1)
    {
	const unsigned short o = memory[PC] & 0x001F;
	const unsigned short b = memory[PC] >> 5 & 0x001F;
	const unsigned short a = memory[PC] >> 10;

	++PC;

	if (o)
	{
	    unsigned short* va = values[a >> 3](a, 1);
	    unsigned short* vb = values[b >> 3](b, 0);
	    opcodes[o](va, vb);
	}
	else if (b)
	{
	    unsigned short* va = values[a >> 3](a, 1);
	    nb_instr[b](va);
	}
    }

    return 0;
}
