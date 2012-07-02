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

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

extern unsigned short memory[0x10000];
extern unsigned short registers[8];
extern unsigned short PC;
extern unsigned short SP;
extern unsigned short EX;

extern unsigned short IA;
extern unsigned short int_queue[256];
extern unsigned char iq_back;
extern unsigned char iq_front;
extern unsigned int int_queueing;

extern unsigned long cycles_counter;
extern unsigned long emu_freq;
extern unsigned int emu_speed;
extern unsigned long nsec_per_chunk;
extern unsigned long cycles_per_chunk;

#endif /* GLOBALS_H_INCLUDED */
