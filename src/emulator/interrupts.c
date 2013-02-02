/*
    DDDCPU16 - DeathlyDeep's DCPU16, a DCPU16 emulator.
    Copyright (C) 2012-2013 Sylvain Boilard <boilard@crans.org>

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

#include "interrupts.h"

unsigned short IA = 0;
unsigned int int_queueing = 0;

unsigned short int_queue[256];
unsigned char iq_back = 0;
unsigned char iq_front = 0;

void recv_int(unsigned short int_val)
{
    static pthread_mutex_t iq_front_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&iq_front_mutex);
    int_queue[iq_front] = int_val;
    ++iq_front; /* Do not increment iq_front until queuing complete. */
    pthread_mutex_unlock(&iq_front_mutex);
}

void trigger_interrupt(void)
{
    if (!int_queueing && iq_front != iq_back)
    {
        if (IA)
        {
            int_queueing = 1;
            memory[--SP] = PC;
            memory[--SP] = registers[0];
            PC = IA;
            registers[0] = int_queue[iq_back];
        }
        ++iq_back;
    }
}
