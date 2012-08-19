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

static void reorder_elem_up(unsigned int index)
{
    while (index)
    {
        unsigned int parent = (index - 1) / 2;
        if (events_heap[index].trigger >= events_heap[parent].trigger)
            return;
        swap_events(index, parent);
        index = parent;
    }
}

static void reorder_elem_down(unsigned int index)
{
    /* If the size of the heap is even, there is a node with only one leaf, so
       we should stop reordering the heap before encountering such a node, and
       treat it specificaly if necessary. This trick does not behave correctly
       if the heap size is 0, but then we do not need to reorder anything. */
    unsigned int half_size = (heap_size - 1) / 2;

    while (index < half_size)
    {
        unsigned int childs = index * 2 + 1;
        unsigned int lower_child = childs +
            (events_heap[childs].trigger >= events_heap[childs + 1].trigger);

        if (events_heap[index].trigger <= events_heap[lower_child].trigger)
            return;
        swap_events(index, lower_child);
        index = lower_child;
    }

    /* Special case for a possible mono-leaf node. */
    if (index == half_size && !(heap_size % 2))
    {
        unsigned int child = index * 2 + 1;

        if (events_heap[index].trigger > events_heap[child].trigger)
            swap_events(index, child);
    }
}

unsigned int get_event_ID(void)
{
    static unsigned int number = 0;
    return number++;
}

void schedule_event(const struct event* event)
{
    if (heap_size == MAX_EVENTS)
    {
        printf("Ignored event scheduling: "
               "no more space available on events heap.");
        return;
    }

    events_heap[heap_size] = *event;
    reorder_elem_up(heap_size++);
}

void cancel_event(unsigned int event_ID, void (* callback)(void*))
{
    unsigned int i;

    for (i = 0; i < heap_size; ++i)
        if (events_heap[i].event_ID == event_ID)
            goto found;
    return;

  found:
    if (--heap_size != i)
    {
        swap_events(i, heap_size);
        reorder_elem_up(i);
        reorder_elem_down(i);
    }

    if (callback)
        callback(events_heap[heap_size].arguments);
}

void trigger_events(void)
{
    while (heap_size && events_heap[0].trigger <= cycles_counter)
    {
        if (--heap_size)
        {
            swap_events(0, heap_size);
            reorder_elem_down(0);
        }
        events_heap[heap_size].callback(events_heap[heap_size].event_ID,
                                        events_heap[heap_size].arguments);
    }
}
