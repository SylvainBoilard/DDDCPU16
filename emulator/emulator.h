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

#ifndef EMULATOR_H_INCLUDED
#define EMULATOR_H_INCLUDED

#define _POSIX_C_SOURCE 200112L
#include <time.h>

#include "globals.h"
#include "opcodes.h"
#include "nb_instr.h"
#include "values.h"

int emulate(void);

#endif /* EMULATOR_H_INCLUDED */
