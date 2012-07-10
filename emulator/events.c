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

#include "events.h"

struct heap_node
{
    struct heap_node* left;
    struct heap_node* right;
    unsigned int balance;
    unsigned int agent_ID;
    unsigned long trigger;
    void (* callback)(unsigned int, void*);
    void* arguments;
};

struct heap_node* events_heap = NULL;

static void heap_insert(struct heap_node** heap, struct heap_node** node)
{
    if ((*heap)->trigger > (*node)->trigger)
    {
	struct heap_node* temp;
	(*node)->left = (*heap)->left;
	(*node)->right = (*heap)->right;
	(*node)->balance = (*heap)->balance;
	temp = *heap;
	*heap = *node;
	*node = temp;
	heap_insert(heap, node);
	return;
    }
    if (!(*heap)->left)
    {
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->balance = 0;
	(*heap)->left = (*node);
	--(*heap)->balance;
	return;
    }
    if (!(*heap)->right)
    {
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->balance = 0;
	(*heap)->right = (*node);
	++(*heap)->balance;
	return;
    }
    if ((*heap)->balance > 0)
    {
	--(*heap)->balance;
	heap_insert(&(*heap)->left, node);
    }
    else
    {
	++(*heap)->balance;
	heap_insert(&(*heap)->right, node);
    }
}

static void heap_pop(struct heap_node** heap)
{
    if (!(*heap)->left)
    {
	if (!(*heap)->right)
	{
	    free(*heap);
	    *heap = NULL;
	}
	else
	{
	    (*heap)->trigger = (*heap)->right->trigger;
	    (*heap)->agent_ID = (*heap)->right->agent_ID;
	    (*heap)->callback = (*heap)->right->callback;
	    (*heap)->arguments = (*heap)->right->arguments;
	    --(*heap)->balance;
	    heap_pop(&(*heap)->right);
	}
	return;
    }
    if (!(*heap)->right)
    {
	(*heap)->trigger = (*heap)->left->trigger;
	(*heap)->agent_ID = (*heap)->left->agent_ID;
	(*heap)->callback = (*heap)->left->callback;
	(*heap)->arguments = (*heap)->left->arguments;
	++(*heap)->balance;
	heap_pop(&(*heap)->left);
	return;
    }
    if ((*heap)->left->trigger < (*heap)->right->trigger)
    {
	(*heap)->trigger = (*heap)->left->trigger;
	(*heap)->agent_ID = (*heap)->left->agent_ID;
	(*heap)->callback = (*heap)->left->callback;
	(*heap)->arguments = (*heap)->left->arguments;
	++(*heap)->balance;
	heap_pop(&(*heap)->left);	
    }
    else
    {
	(*heap)->trigger = (*heap)->right->trigger;
	(*heap)->agent_ID = (*heap)->right->agent_ID;
	(*heap)->callback = (*heap)->right->callback;
	(*heap)->arguments = (*heap)->right->arguments;
	--(*heap)->balance;
	heap_pop(&(*heap)->right);
    }
}

unsigned int get_agent_ID(void)
{
    static unsigned int number = 0;
    return number++;
}

void schedule_event(unsigned int agent_ID, unsigned long trigger,
		    void (* callback)(unsigned int, void*), void* arguments)
{
    struct heap_node* temp =
	(struct heap_node*)malloc(sizeof(struct heap_node));
    temp->trigger = trigger;
    temp->agent_ID = agent_ID;
    temp->callback = callback;
    temp->arguments = arguments;
    if (!events_heap)
    {
	temp->left = NULL;
	temp->right = NULL;
	temp->balance = 0;
	events_heap = temp;
    }
    else
	heap_insert(&events_heap, &temp);
}

void trigger_events(void)
{
    while (events_heap)
    {
	if (events_heap->trigger <= cycles_counter)
	{
	    events_heap->callback(events_heap->agent_ID, events_heap->arguments);
	    heap_pop(&events_heap);
	}
	else
	    return;
    }
}
