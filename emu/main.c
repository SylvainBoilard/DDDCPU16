#include <stdio.h>

#include "globals.h"
#include "opcodes.h"
#include "nb_instr.h"
#include "values.h"

static unsigned int lil_end()
{
    unsigned int test = 0x00000001;
    return *(unsigned char *)&test;
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
	unsigned char temp;
	unsigned char* raw_mem = (unsigned char*)memory;
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
	unsigned short o = memory[PC] & 0x000F;
	unsigned short a = memory[PC] >> 4 & 0x003F;
	unsigned short b = memory[PC] >> 10;

	++PC;

	if (o)
	{
	    unsigned short* va = values[a >> 3](a);
	    unsigned short* vb = values[b >> 3](b);;
	    opcodes[o](va, vb);
	}
	else if (a)
	{
	    unsigned short* vb = values[b >> 3](b);
	    nb_instr[a](vb);
	}
	else
	{
	    /* Reserved for future expansion. */
	}
    }

    return 0;
}
