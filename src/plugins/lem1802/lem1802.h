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

#ifndef LEM1802_H_INCLUDED
#define LEM1802_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <GL/gl.h>
#include <SFML/System.h>
#include <SFML/Window.h>

#include <dddcpu16.h>

#include "defaults.h"

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[]);
void term(void);

struct lem1802_context
{
    sfWindow* window;
    unsigned short screen_map;
    unsigned short font_map;
    unsigned short palette_map;
    unsigned short border_color;
    unsigned int heating;
};

#endif // LEM1802_H_INCLUDED
