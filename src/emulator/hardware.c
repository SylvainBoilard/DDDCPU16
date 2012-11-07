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

#include "hardware.h"

struct hardware_node* hard_stack = NULL;

struct hardware* hard_array = NULL;
unsigned int hard_number = 0;

void add_hard(void (* hard_info_callback)(void),
              unsigned int (* hard_send_int_callback)(unsigned short),
              unsigned short hard_PCID)
{
    struct hardware_node* hard_node_temp;

    hard_node_temp =
        (struct hardware_node*)malloc(sizeof(struct hardware_node));
    hard_node_temp->hard.hard_info = hard_info_callback;
    hard_node_temp->hard.hard_send_int = hard_send_int_callback;
    hard_node_temp->hard.hard_PCID = hard_PCID;

    hard_node_temp->next = hard_stack;
    hard_stack = hard_node_temp;
    ++hard_number;
}

int load_hard(void)
{
    unsigned int i = hard_number;

    if (hard_number >= 0x10000)
    {
        printf("Too many pieces of hardware plugged-in (%u).\n", hard_number);
        return 1;
    }

    hard_array =
        (struct hardware*)malloc(sizeof(struct hardware) * hard_number);
    while (i--)
    {
        struct hardware_node* hard_node = hard_stack;
        hard_stack = hard_stack->next;
        hard_array[i] = hard_node->hard;
        free(hard_node);
    }
    return 0;
}

void free_hard(void)
{
    if (hard_array)
        free(hard_array);
    else
        while (hard_stack)
        {
            struct hardware_node* current_node = hard_stack;
            hard_stack = hard_stack->next;
            free(current_node);
        }
}

unsigned short hard_count(void)
{
    return hard_number;
}

void hard_info(unsigned short hard_no)
{
    if (hard_no < hard_number)
        hard_array[hard_no].hard_info();
}

unsigned long hard_send_int(unsigned short hard_no)
{
    if (hard_no < hard_number)
        return hard_array[hard_no].hard_send_int(hard_array[hard_no].hard_PCID);
    return 0;
}
