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
unsigned long chunk_start;
unsigned long event_ID;
unsigned long trigger;

static void emu_sleep(void* arguments)
{
    struct timespec current;
    struct timespec sleep;

    chunk_start += emu_granularity;
    trigger += cycles_per_chunk;
    event_ID = schedule_event(trigger, emu_sleep, NULL);

    clock_gettime(CLOCK_MONOTONIC, &current);
    sleep.tv_nsec = chunk_start - current.tv_sec * 1000000000 - current.tv_nsec;
    if (sleep.tv_nsec > 0)
    {
        /* emu_granularity is strictly lower than 1 second, so if we
           need to sleep, it will always be less than 1 second. */
        sleep.tv_sec = 0;
        nanosleep(&sleep, NULL);
    }
}

void init_timing(void)
{
    struct timespec temp;

    /* These operations must be done in an order
       such that it minimizes rounding errors. */
    cycles_per_chunk =
        emu_freq * emu_speed / 1000 * emu_granularity / 1000000000;

    trigger = cycles_per_chunk;
    event_ID = schedule_event(trigger, emu_sleep, NULL);

    clock_gettime(CLOCK_MONOTONIC, &temp);
    chunk_start = temp.tv_sec * 1000000000 + temp.tv_sec;
}

void term_timing(void)
{
    cancel_event(event_ID, NULL);
}
