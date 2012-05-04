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

#include "values.h"

static unsigned short* reg(unsigned short v, unsigned short is_a)
{
    return registers + (v & 0x0007);
}

static unsigned short* regad(unsigned short v, unsigned short is_a)
{
    return memory + registers[v & 0x0007];
}

static unsigned short* nwreg(unsigned short v, unsigned short is_a)
{
    ++cycles;
    return memory + (unsigned short)(memory[PC++] + registers[v & 0x0007]);
}

static unsigned short* adv(unsigned short v, unsigned short is_a)
{
    return advanced[v & 0x0007](is_a);
}

static unsigned short* lit(unsigned short v, unsigned short is_a)
{
    static unsigned short value;
    if (v == 0x003F)
	value = 0xFFFF;
    else
	value = v & 0x001F;
    return &value;
}

unsigned short*(* const values[])(unsigned short v, unsigned short is_a) = {
    reg, regad, nwreg, adv, lit, lit, lit, lit
};
