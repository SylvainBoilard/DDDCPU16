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

#include "timing.h"

unsigned long cycles_per_chunk;
struct event emu_sleep_event;
struct timespec chunk_start;

static void emu_sleep(unsigned int event_ID, void* arguments)
{
    struct timespec current;
    struct timespec sleep;

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
    emu_sleep_event.trigger += cycles_per_chunk;
    schedule_event(&emu_sleep_event);
}

void init_timing(void)
{
    /* These operations must be done in an order
       such that it minimizes rounding errors. */
    cycles_per_chunk =
	emu_freq *
	emu_speed / 1000 *
	nsec_per_chunk / 1000000000;

    emu_sleep_event.trigger = cycles_per_chunk;
    emu_sleep_event.event_ID = get_event_ID();
    emu_sleep_event.callback = emu_sleep;
    schedule_event(&emu_sleep_event);
    clock_gettime(CLOCK_MONOTONIC, &chunk_start);
}

void term_timing(void)
{
    cancel_event(emu_sleep_event.event_ID, NULL);
}
