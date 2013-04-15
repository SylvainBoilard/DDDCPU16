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

#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "init.h"

#define COMMAND_BUFFER_SIZE 256

struct command
{
    const char* name;
    void (* callback)(unsigned int, const char*[]);
};

struct command_node
{
    struct command command;
    struct command_node* next;
};

struct uint_node
{
    unsigned int value;
    struct uint_node* next;
};

void add_command(const char* name,
                 void (* callback)(unsigned int, const char*[]));
void init_console(void);
void term_console(void);

#endif // CONSOLE_H_INCLUDED
