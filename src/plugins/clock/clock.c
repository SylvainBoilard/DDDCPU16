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

#include "clock.h"

struct dddcpu16_context context;
unsigned short clock_number = 0;
struct clock_context* clock_array = NULL;

static void info(void)
{
    context.registers[0] = 0xb402;
    context.registers[1] = 0x12d0;
    context.registers[2] = 0;
    context.registers[3] = 0;
    context.registers[4] = 0;
}

static void tick(unsigned int event_ID, void* arguments)
{
    struct clock_context* current_clock = (struct clock_context*)arguments;

    if (current_clock->interrupt)
        context.send_int(current_clock->interrupt);
    current_clock->event.trigger += current_clock->cycles_per_tick;
    context.schedule_event(&current_clock->event);
}

static unsigned int recv_int(unsigned short PCID)
{
    switch (context.registers[0])
    {
    case 0:
        clock_array[PCID].last_set = *context.cycles_counter;
        context.cancel_event(clock_array[PCID].event.event_ID, NULL);
        if (context.registers[1])
        {
            clock_array[PCID].cycles_per_tick =
                *context.emu_freq * 60 / context.registers[1];
            clock_array[PCID].event.trigger =
                *context.cycles_counter + clock_array[PCID].cycles_per_tick;
            context.schedule_event(&clock_array[PCID].event);
        }

        break;

    case 1:
        context.registers[2] =
            *context.cycles_counter - clock_array[PCID].last_set;
        break;

    case 2:
        clock_array[PCID].interrupt = context.registers[1];

    default:;
    }

    return 0;
}

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[])
{
    context = *dddcpu16_context;
    ++clock_number;
    return 0;
}

void complete_load(void)
{
    unsigned int i;

    clock_array = (struct clock_context*)
        malloc(sizeof(struct clock_context) * clock_number);
    for (i = 0; i < clock_number; ++i)
    {
        struct hardware hardware = {info, recv_int, 0};
        struct clock_context base_context = {
            0, 0, 0, {0, 0, tick, NULL}
        };

        hardware.hard_PCID = i;
        context.add_hard(&hardware);

        base_context.event.event_ID = context.get_event_ID();
        base_context.event.arguments = (void*)clock_array;
        clock_array[i] = base_context;
    }
}

void term(void)
{
    if (clock_array)
        free(clock_array);
}
