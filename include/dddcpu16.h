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

struct dddcpu16_context
{
    unsigned short* memory;
    unsigned short* registers;
    const unsigned long* cycles_counter;
    unsigned long emu_freq; /* In Hertz. */
    unsigned int emu_speed; /* Speed factor. */
    unsigned int emu_granularity; /* In nanoseconds. */

    /* add_hard() takes three arguments:
       - a pointer to a function whos job is to load the registers with the
         hardware informations.
       - a pointer to a function whos job is to handle interrupts sent to a
         piece of hardware designated by a plugin-side ID and returning the
         number of cycles needed to do so.
       - a plugin-side ID to differenciate between numerous instances of the
         same piece of hardware. */
    void (* add_hard)(void (*)(void), unsigned int (*)(unsigned short),
                      unsigned short);
    /* add_command takes two arguments:
       - the name of the command to add.
       - a pointer to a function to call when the command is issued, taking the
         number of arguments plus one and an array containing the command and
         its arguments as arguments. */
    void (* add_command)(const char *, void (*)(unsigned int, const char*[]));
    /* send_int() takes the interrupt value as argument. */
    void (* send_int)(unsigned short);
    /* schedule_event() takes three arguments:
       - a cycle number at which the event will be triggered.
       - a pointer to a function which will be called when the event is
         triggered.
       - a pointer which will be passed as an argument to the previously
         provided function when this one will be called.
       Returns an event ID, used to cancel events when necessary. The returned
       ID will never be 0, except if more than 2^64 events are scheduled in one
       session. Very unlikely. */
    unsigned long (* schedule_event)(unsigned long, void (*)(void*), void*);
    /* cancel_event() takes two arguments:
       - the event ID of the event to cancel.
       - a pointer to a function whos job is to free the resources allocated
         for the arguments pointer associated with the event. Can be NULL. */
    void (* cancel_event)(unsigned long, void (*)(void*));
};

#endif /* DDDCPU16_H_INCLUDED */
