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

struct hardware* hd_hard = NULL;
unsigned int hd_number = 0;

struct hardware_node
{
    struct hardware_node* next;
    struct hardware hard;
};

static struct hardware_node* hd_stack = NULL;

unsigned int add_hard(const struct hardware* hardware)
{
    struct hardware_node* hd_node_tmp =
	(struct hardware_node*)malloc(sizeof(struct hardware_node));
    hd_node_tmp->hard = *hardware;
    hd_node_tmp->next = hd_stack;
    hd_stack = hd_node_tmp;
    return hd_number++;
}

void complete_load_hard(void)
{
    unsigned int i = hd_number;

    hd_hard = (struct hardware*)malloc(sizeof(struct hardware) * hd_number);
    /* Since the new pieces of hardware are pushed onto a stack during the
       initialization, when we need to unfold it to put the hardware in an
       array, the hardware on top of the stack has the highest index, so we have
       to start from the end of the array. */
    while (i--)
    {
	struct hardware_node* hard_node = hd_stack;
	hd_stack = hd_stack->next;
	hd_hard[i] = hard_node->hard;
	free(hard_node);
    }
}

void free_hard(void)
{
    if (hd_hard)
	free(hd_hard);
    else
	while (hd_stack)
	{
	    struct hardware_node* current_node = hd_stack;
	    hd_stack = hd_stack->next;
	    free(current_node);
	}
}
