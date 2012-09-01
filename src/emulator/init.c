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

/* Returns 1 if host uses little-endian, 0 overwise. */
static unsigned int host_endn(void)
{
    const unsigned int test = 0x00000001;
    return *(const unsigned char *)&test;
}

static int load_ram(const char* file, unsigned int file_endn)
{
    FILE* ram_img;

    ram_img = fopen(file, "rb");
    if (!ram_img)
        return 1;
    fread(memory, 1, 0x20000, ram_img);
    fclose(ram_img);
    if (host_endn() != file_endn)
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
    unsigned int ram_image_endn = 1; /* Default is little endian. */
    int i;

    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            int curr_arg = i;
            int ret_val;
            long value;

            switch (argv[i][1])
            {
            case 'B': /* Ram image's endian-ness options. */
                ram_image_endn = 0;
                break;

            case 'L':
                ram_image_endn = 1;
                break;

            case 'G': /* Granularity for emulation timing option. */
                if (++i >= argc)
                {
                    printf("You need to precise a granularity "
                           "with option -G.\n");
                    return 1;
                }
                value = read_ns(argv[i]);

                if (value < 0)
                {
                    printf("Error reading granularity: %s.\n", argv[i]);
                    return 1;
                }
                if (!value)
                {
                    printf("Cannot set granularity to 0 nanosecond.\n");
                    return 1;
                }
                if (value >= 1000000000)
                {
                    printf("Cannot set granularity greater than 1 second.\n");
                    return 1;
                }

                emu_granularity = value;
                break;

            case 's': /* Emulation speed option. */
                if (++i >= argc)
                {
                    printf("You need to precise a speed with option -s.\n");
                    return 1;
                }
                value = read_float_10E3(argv[i]);

                if (value < 0)
                {
                    printf("Error reading emulation speed: %s.\n", argv[i]);
                    return 1;
                }
                if (!value)
                {
                    printf("Cannot set emulation speed to 0.\n");
                    return 1;
                }

                emu_speed = value;
                break;

            case 'f': /* Emulation frequency option. */
                if (++i >= argc)
                {
                    printf("You need to precise a frequency with option -f.\n");
                    return 1;
                }
                value = read_hz(argv[i]);

                if (value < 0)
                {
                    printf("Error reading emulation frequency: %s.\n", argv[i]);
                    return 1;
                }
                if (!value)
                {
                    printf("Cannot set emulation frequency to 0 Hz.\n");
                    return 1;
                }

                emu_freq = value;
                break;

            case 'p': /* Plugin loading option. */
                ++curr_arg;
                while (++i < argc && strcmp(argv[i], "--"));
                ret_val = load_plugin(i - curr_arg, argv + curr_arg);
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
    if (load_ram(argv[ram_image_index], ram_image_endn))
    {
        printf("Cannot open %s.\n", argv[ram_image_index]);
        return 2;
    }

    init_timing();
    complete_load_hard();
    complete_load_plugins();

    return 0;
}

void term(void)
{
    term_timing();
    free_hard();
    free_plugins();
}
