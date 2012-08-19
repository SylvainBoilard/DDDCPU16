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

#include "adv.h"

#include <stdio.h>

static unsigned short* popush(unsigned short is_a)
{
    return memory + (is_a ? SP++ : --SP);
}

static unsigned short* peek(unsigned short is_a)
{
    return memory + SP;
}

static unsigned short* pick(unsigned short is_a)
{
    ++cycles_counter;
    return memory + (unsigned short)(SP + memory[PC++]);
}

static unsigned short* sp(unsigned short is_a)
{
    return &SP;
}

static unsigned short* pc(unsigned short is_a)
{
    return &PC;
}

static unsigned short* ex(unsigned short is_a)
{
    return &EX;
}

static unsigned short* nw(unsigned short is_a)
{
    ++cycles_counter;
    return memory + memory[PC++];
}

static unsigned short* nwlit(unsigned short is_a)
{
    static unsigned short value;
    value = memory[PC++];
    ++cycles_counter;
    return &value;
}

unsigned short* (* const advanced[])(unsigned short is_a) = {
    popush, peek, pick, sp, pc, ex, nw, nwlit
};
