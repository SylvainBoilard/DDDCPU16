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

struct hardware* hard_array = NULL;
unsigned int hard_number = 0;

struct hardware_node
{
    struct hardware_node* next;
    struct hardware hard;
};

static struct hardware_node* hard_stack = NULL;

void add_hard(const struct hardware* hardware)
{
    struct hardware_node* hard_node_temp =
        (struct hardware_node*)malloc(sizeof(struct hardware_node));
    hard_node_temp->hard = *hardware;
    hard_node_temp->next = hard_stack;
    hard_stack = hard_node_temp;
    ++hard_number;
}

void complete_load_hard(void)
{
    unsigned int i;

    hard_array =
        (struct hardware*)malloc(sizeof(struct hardware) * hard_number);
    for (i = 0; i < hard_number; ++i)
    {
        struct hardware_node* hard_node = hard_stack;
        hard_stack = hard_stack->next;
        hard_array[i] = hard_node->hard;
        free(hard_node);
    }
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
