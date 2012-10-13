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

#ifndef PLUGIN_H_INCLUDED
#define PLUGIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include <dddcpu16.h>

#include "globals.h"
#include "events.h"
#include "interrupts.h"
#include "hardware.h"

struct plugin_args_node
{
    struct plugin_args_node* next;
    int argc;
    char** argv;
};

struct plugin_node
{
    struct plugin_node* next;
    void* dl_handle;
};

void add_plugin(int plugin_argc, char* plugin_argv[]);
int load_plugins(void);
void free_plugins(void);

#endif /* PLUGIN_H_INCLUDED */
