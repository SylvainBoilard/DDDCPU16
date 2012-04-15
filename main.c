#include <stdio.h>

#include "globals.h"
#include "opcodes.h"
#include "values.h"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
	FILE* fichier = fopen(argv[1], "r");
	if (!fichier)
	{
	    printf("Impossible d’ouvrir %s.", argv[1]);
	    return 1;
	}
	fread(memory, 2, 0x10000, fichier);
	fclose(fichier);
    }

    while (1)
    {
	unsigned short o = memory[PC] & 0x000F;
	unsigned short a = memory[PC] >> 4 & 0x003F;
	unsigned short b = memory[PC] >> 10;
	unsigned short* va, * vb;

	++PC;
	va = values[a >> 3](a);
	vb = values[b >> 3](b);
	opcodes[o](va, vb);
    }

    return 0;
}
