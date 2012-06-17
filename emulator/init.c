/*
    DDDCPU16 - DeathlyDeep's DCPU16, a DCPU16 emulator.
    Copyright (C) 2012 Sylvain BOILARD <boilard@crans.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "init.h"

static unsigned int lil_end(void)
{
    const unsigned int test = 0x00000001;
    return *(const unsigned char *)&test;
}

static int load_ram(const char* file)
{
    FILE* ram_img;

    ram_img = fopen(file, "rb");
    if (!ram_img)
	return 1;
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
    return 0;
}

int init(int argc, char* argv[])
{
    unsigned int ram_image_index = 0;
    int i;

    for (i = 1; i < argc; ++i)
    {
	if (argv[i][0] == '-')
	{
	    int curr_arg = i;
	    int ret_val;

	    switch (argv[i][1])
	    {
	    case 'h':
		while (++i < argc && strcmp(argv[i], "--"));
		ret_val = load_hard(i - curr_arg, argv + curr_arg);
		if (ret_val)
		    return ret_val;
		break;

	    default:
		printf("Unknown option: %s.\n", argv[i]);
		return 1;
	    }
	}
	else if (!ram_image_index)
	    ram_image_index = i;
	else
	{
	    printf("There is more than one ram image specified.\n");
	    return 1;
	}
    }

    if (!ram_image_index)
    {
	printf("You must specify a RAM image to load.\n");
	return 1;
    }
    if (load_ram(argv[ram_image_index]))
    {
	printf("Cannot open %s.\n", argv[ram_image_index]);
	return 2;
    }

    complete_load_hard();

    return 0;
}

void term(void)
{
    free_hard();
}
