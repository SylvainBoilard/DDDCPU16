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

#ifndef DDDCPU16_H_INCLUDED
#define DDDCPU16_H_INCLUDED

struct event
{
    unsigned long trigger;
    unsigned int agent_ID;
    void (* callback)(unsigned int, void*);
    void* arguments;
};

struct dddcpu16_context
{
    unsigned short* memory;
    unsigned short* registers;
    const unsigned long* emu_freq;
    const unsigned int* emu_speed;
    const unsigned long* cycles_counter;
    void (* send_int)(unsigned short);
    unsigned int (* get_agent_ID)(void);
    void (* schedule_event)(const struct event*);
    void (* cancel_event)(unsigned int, void (*)(void*));
};

#endif /* DDDCPU16_H_INCLUDED */
