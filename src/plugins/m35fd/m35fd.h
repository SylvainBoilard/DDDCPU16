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

#ifndef M35FD_H_INCLUDED
#define M35FD_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#include <dddcpu16.h>

#define FLOPPY_SIZE 1474560

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[]);
void term(void);

enum m35fd_states
{
    STATE_NO_MEDIA,
    STATE_READY,
    STATE_READY_WP,
    STATE_BUSY
};

enum m35fd_errors
{
    ERROR_NONE,
    ERROR_BUSY,
    ERROR_NO_MEDIA,
    ERROR_PROTECTED,
    ERROR_EJECT,
    ERROR_BAD_SECTOR,
    ERROR_BROKEN = 0xffff
};

struct m35fd_context
{
    enum m35fd_states state;
    enum m35fd_errors last_error;
    unsigned short interrupt;
    unsigned char write_protected;
    unsigned char current_track;
    unsigned int is_read;
    unsigned long event_ID;
    unsigned short disk_sector;
    unsigned short memory_location;
    int floppy_fd;
    unsigned short* floppy_map;
    pthread_mutex_t lock;
};

#endif /* M35FD_H_INCLUDED */
