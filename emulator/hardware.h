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

#ifndef HARDWARE_H_INCLUDED
#define HARDWARE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "globals.h"

struct hardware
{
    void (* hd_info)(void);
    unsigned int (* hd_send_int)(void);
    void* dl_handle;
};

extern struct hardware* hd_hard;
extern unsigned int hd_number;

int load_hard(int hard_argc, char* hard_argv[]);
void complete_load_hard(void);
void free_hard(void);

#endif /* HARDWARE_H_INCLUDED */
