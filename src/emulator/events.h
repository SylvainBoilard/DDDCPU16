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

#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <stdio.h>

#include <dddcpu16.h>

#include "globals.h"

unsigned int get_event_ID(void);
void schedule_event(const struct event* event);
void cancel_event(unsigned int event_ID, void (* callback)(void*));
void trigger_events(void);

#endif /* EVENTS_H_INCLUDED */
