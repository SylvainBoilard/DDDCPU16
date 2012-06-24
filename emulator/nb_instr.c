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

#include "nb_instr.h"

static void jsr(unsigned short* a)
{
    memory[--SP] = PC;
    PC = *a;
    cycles += 3;
}

static void swi(unsigned short* a) /* Called INT in spec. */
{
    recv_int(*a);
    cycles += 4;
}

static void iag(unsigned short* a)
{
    *a = IA;
    ++cycles;
}

static void ias(unsigned short* a)
{
    IA = *a;
    ++cycles;
}

static void rfi(unsigned short* a)
{
    int_queueing = 0;
    registers[0] = memory[SP++];
    PC = memory[SP++];
    cycles += 3;
}

static void iaq(unsigned short* a)
{
    int_queueing = *a;
    cycles += 2;
}

static void hwn(unsigned short* a)
{
    *a = hd_number;
    cycles += 2;
}

static void hwq(unsigned short* a)
{
    unsigned short hard_no = *a;
    if (hard_no < hd_number)
	hd_hard[hard_no].hd_info();
    cycles += 4;
}

static void hwi(unsigned short* a)
{
    unsigned short hard_no = *a;
    if (hard_no < hd_number)
	cycles += hd_hard[hard_no].hd_send_int();
    cycles += 4;
}

static void NONE(unsigned short* a)
{
    ++cycles; /* See emulator.c for explanations. */
}

void (* const nb_instr[])(unsigned short* a) = {
    (void*)0, jsr, NONE, NONE, NONE, NONE, NONE, NONE,
    swi, iag, ias, rfi, iaq, NONE, NONE, NONE,
    hwn, hwq, hwi, NONE, NONE, NONE, NONE, NONE,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
};
