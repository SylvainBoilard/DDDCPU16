/*
    DDDCPU16 - DeathlyDeep's DCPU16, a DCPU16 emulator.
    Copyright (C) 2012-2013 Sylvain Boilard <boilard@crans.org>

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

#include "lem1802.h"

struct dddcpu16_context context;
unsigned int lem1802_number = 0;
struct lem1802_context* lem1802_array = NULL;

/*static int read_uint(const char* string)
{
    unsigned int result = 0;

    if (*string < '0' || *string > '9')
        return -1;

    do
        result = result * 10 + *string - '0';
    while (*++string >= '0' && *string <= '9');

    if (*string)
        return -1;
    return result;
}

static unsigned int recv_int(unsigned short PCID)
{
    return 0;
}

static void info(void)
{
    context.registers[0] = 0xf615;
    context.registers[1] = 0x7349;
    context.registers[2] = 0x1802;
    context.registers[3] = 0x8b36;
    context.registers[4] = 0x1c6c;
}*/

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[])
{
    context = *dddcpu16_context;

    return 0;
}

void term(void)
{
    if (lem1802_array)
    {
        unsigned int i;
        for (i = 0; i < lem1802_number; ++i)
            ;
        free(lem1802_array);
    }
}
