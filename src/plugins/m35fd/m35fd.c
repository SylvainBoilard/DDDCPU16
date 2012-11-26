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

#include "m35fd.h"

struct dddcpu16_context context;
unsigned int m35fd_number = 0;
struct m35fd_context* m35fd_array = NULL;

static int read_uint(const char* string)
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

static void info(void)
{
    context.registers[0] = 0x24c5;
    context.registers[1] = 0x4fd5;
    context.registers[2] = 0x000b;
    context.registers[3] = 0x7e91;
    context.registers[4] = 0x1eb3;
}

static unsigned int recv_int(unsigned short PCID)
{
    switch (context.registers[0])
    {
    default:;
    }

    return 0;
}

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[])
{
    unsigned int i;
    int result;
    context = *dddcpu16_context;

    if (argc > 1)
    {
        result = read_uint(argv[1]);
        if (result < 0)
        {
            printf("Could not read m35fd number \"%s\".\n", argv[1]);
            return 1;
        }
        if (!result)
        {
            printf("Cannot set m35fd number to 0.\n");
            return 1;
        }
        m35fd_number = result;
    }
    else
        m35fd_number = 1;

    m35fd_array = (struct m35fd_context*)
        malloc(sizeof(struct m35fd_context) * m35fd_number);
    for (i = 0; i < m35fd_number; ++i)
    {
        context.add_hard(info, recv_int, i);
        m35fd_array[i].state = STATE_NO_MEDIA;
        m35fd_array[i].last_error = ERROR_NONE;
        pthread_mutex_init(&m35fd_array[i].lock, NULL);
    }

    return 0;
}

void term(void)
{
    unsigned int i;
    if (m35fd_array)
    {
        for (i = 0; i < m35fd_number; ++i)
        {
            /* TODO : Free contexts related ressources correctly. */
        }
        free(m35fd_array);
    }
}
