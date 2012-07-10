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

#define MAX_EVENTS 1024 /* TODO : add option to modify this value. */

/* events_heap is a binary min-heap storing events against their trigger value.
   See <https://en.wikipedia.org/wiki/Binary_heap> */
struct event events_heap[MAX_EVENTS];
unsigned int heap_size = 0;

static void swap_events(unsigned int first, unsigned int second)
{
    struct event temp = events_heap[first];
    events_heap[first] = events_heap[second];
    events_heap[second] = temp;
}

unsigned int get_agent_ID(void)
{
    static unsigned int number = 0;
    return number++;
}

void schedule_event(const struct event* event)
{
    unsigned int current_elem = heap_size;

    if (heap_size == MAX_EVENTS)
    {
	printf("Ignored event scheduling: "
	       "no more space available on events heap.");
	return;
    }

    events_heap[heap_size++] = *event;
    while (current_elem)
    {
	unsigned int parent = (current_elem - 1) / 2;
	if (events_heap[current_elem].trigger >= events_heap[parent].trigger)
	    return;
	swap_events(current_elem, parent);
	current_elem = parent;
    }
}

void trigger_events(void)
{
    while (heap_size && events_heap[0].trigger >= cycles_counter)
    {
	if (--heap_size)
	{
	    unsigned int current_elem = 0;
	    /* If the size of the heap is even, there is a node with only one
	       leaf, so we should stop reordering the heap before encountering
	       such a node, and treat it specificaly if necessary. */
	    unsigned int half_size = (heap_size - 1) / 2;

	    swap_events(0, heap_size);

	    while (current_elem < half_size)
	    {
		unsigned int childs = current_elem * 2 + 1;
		unsigned int lower_child = childs +
		    (events_heap[childs].trigger >=
		     events_heap[childs + 1].trigger);

		if (events_heap[current_elem].trigger >
		    events_heap[lower_child].trigger)
		{
		    swap_events(current_elem, lower_child);
		    current_elem = lower_child;
		}
		else /* The heap is reordered, we can move on. */
		    goto execute_event_callback;
	    }

	    /* Special case for a possible mono-leaf node. */
	    if (current_elem == half_size && !(heap_size % 2))
	    {
		unsigned int child = current_elem * 2 + 1;

		if (events_heap[current_elem].trigger >
		    events_heap[child].trigger)
		{
		    swap_events(current_elem, child);
		}
	    }
	}

      execute_event_callback:
	events_heap[heap_size].callback(events_heap[heap_size].agent_ID,
					events_heap[heap_size].arguments);
    }
}
