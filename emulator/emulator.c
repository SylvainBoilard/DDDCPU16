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

#include "emulator.h"

int emulate(void)
{
    unsigned long last_sleep = cycles_counter;
    struct timespec chunk_start;

    clock_gettime(CLOCK_MONOTONIC, &chunk_start);

    while (1)
    {
	const unsigned short o = memory[PC] & 0x001F;
	const unsigned short b = memory[PC] >> 5 & 0x001F;
	const unsigned short a = memory[PC] >> 10;

	++PC;

	if (o)
	{
	    const unsigned short* va = values[a >> 3](a, 1);
	    unsigned short* vb = values[b >> 3](b, 0);
	    opcodes[o](vb, va);
	}
	else if (b)
	{
	    unsigned short* va = values[a >> 3](a, 1);
	    nb_instr[b](va);
	}
	else
	    /* These opcodes have currently no purpose. We just spend a cycle
	       doing nothing so that the instruction lasts a non-null period of
	       time to prevent running through the RAM at full speed in case the
	       RAM is full of those. */
	    ++cycles_counter;

	trigger_interrupt();
	trigger_events();

	if (cycles_counter - last_sleep >= cycles_per_chunk)
	{
	    struct timespec current;
	    struct timespec sleep;

	    last_sleep += cycles_per_chunk;
	    clock_gettime(CLOCK_MONOTONIC, &current);
	    sleep.tv_nsec = nsec_per_chunk -
		(current.tv_nsec - chunk_start.tv_nsec) -
		(current.tv_sec - chunk_start.tv_sec) * 1000000000;
	    if (sleep.tv_nsec > 0)
	    {
		sleep.tv_sec = 0;
		nanosleep(&sleep, NULL);
	    }

	    chunk_start.tv_nsec += nsec_per_chunk;
	    if (chunk_start.tv_nsec >= 1000000000)
	    {
		chunk_start.tv_nsec -= 1000000000;
		++chunk_start.tv_sec;
	    }
	}
    }

    return 0;
}
