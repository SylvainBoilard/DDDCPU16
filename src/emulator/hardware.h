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

struct hardware
{
    void (* hard_info)(void);
    unsigned int (* hard_send_int)(unsigned short);
    unsigned short hard_PCID;
};

struct hardware_node
{
    struct hardware_node* next;
    struct hardware hard;
};

void add_hard(void (* hard_info_callback)(void),
              unsigned int (* hard_send_int_callback)(unsigned short),
              unsigned short hard_PCID);
void load_hard(void);
void free_hard(void);
unsigned short hard_count(void);
void hard_info(unsigned short hard_no);
unsigned long hard_send_int(unsigned short hard_no);

#endif /* HARDWARE_H_INCLUDED */
