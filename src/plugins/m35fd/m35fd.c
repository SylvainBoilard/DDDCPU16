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

#include "m35fd.h"

#define abs(x) (x < 0 ? -x : x)

struct dddcpu16_context context;
unsigned int m35fd_number = 0;
struct m35fd_context* m35fd_array = NULL;

static int read_uint(const char* string)
{
    unsigned int result = 0;

    if (*string < '0' || *string > '9')
        return -1;

    do
        result = result * 10 + *string - '0';
    while (*++string >= '0' && *string <= '9');

    if (*string)
        return -1;
    return result;
}

static void info(void)
{
    context.registers[0] = 0x24c5;
    context.registers[1] = 0x4fd5;
    context.registers[2] = 0x000b;
    context.registers[3] = 0x7e91;
    context.registers[4] = 0x1eb3;
}

static void cmd_insert(unsigned int argc, const char* argv[])
{
    unsigned int drive_ID = 0;
    unsigned int write_protect = 0;
    unsigned int i;

    if (argc < 2)
    {
        printf("Usage: m35fd.insert <filename> [-wp] [drive number]\n");
        return;
    }

    for (i = 2; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-wp"))
            write_protect = 1;
        else
        {
            drive_ID = read_uint(argv[i]);
            if (drive_ID == (unsigned int)-1)
            {
                printf("Could not read m35fd number \"%s\"\n.", argv[i]);
                return;
            }
        }
    }

    if (drive_ID >= m35fd_number)
    {
        printf("Unknown m35fd number %i. There is currently %u m35fd "
               "connected to the computer.\n", drive_ID, m35fd_number);
        return;
    }

    if (m35fd_array[drive_ID].state != STATE_NO_MEDIA)
    {
        printf("There is already a floppy in drive %u. "
               "You can eject it with the m35fd.eject command.\n", drive_ID);
        return;
    }

    pthread_mutex_lock(&m35fd_array[drive_ID].lock);

    m35fd_array[drive_ID].floppy_fd =
        open(argv[1], write_protect ? O_RDONLY : (O_RDWR | O_CREAT));
    if (m35fd_array[drive_ID].floppy_fd < 0)
        printf("Could not open file %s.\n", argv[1]);
    else
    {
        m35fd_array[drive_ID].state = STATE_READY + write_protect;
        m35fd_array[drive_ID].write_protected = write_protect;
    }

    pthread_mutex_unlock(&m35fd_array[drive_ID].lock);
}

static void cmd_eject(unsigned int argc, const char* argv[])
{
    unsigned int drive_ID = 0;

    if (argc > 1)
    {
        drive_ID = read_uint(argv[1]);
        if (drive_ID == (unsigned int)-1)
        {
            printf("Could not read m35fd number \"%s\"\n.", argv[1]);
            return;
        }

        if (drive_ID >= m35fd_number)
        {
            printf("Unknown m35fd number %i. There is currently %u m35fd "
                   "connected to the computer.\n", drive_ID, m35fd_number);
            return;
        }
    }

    if (m35fd_array[drive_ID].state == STATE_NO_MEDIA)
    {
        printf("There is no floppy in drive %u.\n", drive_ID);
        return;
    }

    pthread_mutex_lock(&m35fd_array[drive_ID].lock);

    if (m35fd_array[drive_ID].state == STATE_BUSY)
    {
        context.cancel_event(m35fd_array[drive_ID].event_ID, NULL);
        m35fd_array[drive_ID].last_error = ERROR_EJECT;
        if (m35fd_array[drive_ID].interrupt)
            context.send_int(m35fd_array[drive_ID].interrupt);
    }
    m35fd_array[drive_ID].state = STATE_NO_MEDIA;
    close(m35fd_array[drive_ID].floppy_fd);

    pthread_mutex_unlock(&m35fd_array[drive_ID].lock);
}

static void do_action(void* argument)
{
    struct m35fd_context* current_m35fd = (struct m35fd_context*)argument;

    pthread_mutex_lock(&current_m35fd->lock);
    if (current_m35fd->state != STATE_BUSY)
    {
        /* The disk was ejected between the moment this event was triggered and
           the moment we actually took the lock. It is even possible (although
           very unlikely) that another disk was inserted in the mean time. */
        pthread_mutex_unlock(&current_m35fd->lock);
        return;
    };

    lseek(current_m35fd->floppy_fd, current_m35fd->disk_sector * 1024,SEEK_SET);
    /* TODO : Handle endianness and wrapping at the end of memory. */
    if (current_m35fd->is_read)
    {
        unsigned int amount_read =
            read(current_m35fd->floppy_fd,
                 context.memory + current_m35fd->memory_location, 1024);
        for (; amount_read < 1024; ++amount_read)
            ((char*)context.memory)[amount_read] = 0;
    }
    else
        write(current_m35fd->floppy_fd,
              context.memory + current_m35fd->memory_location, 1024);

    current_m35fd->state = STATE_READY + current_m35fd->write_protected;
    if (current_m35fd->interrupt)
        context.send_int(current_m35fd->interrupt);

    pthread_mutex_unlock(&current_m35fd->lock);
}

static void schedule_action(unsigned int is_read, unsigned short disk_sector,
                            unsigned short memory_location, unsigned short PCID)
{
    unsigned long do_cycle;
    int tracks_delta;

    switch (m35fd_array[PCID].state)
    {
    case STATE_NO_MEDIA:
        m35fd_array[PCID].last_error = ERROR_NO_MEDIA;
        goto action_error;

    case STATE_BUSY:
        m35fd_array[PCID].last_error = ERROR_BUSY;
        goto action_error;

    case STATE_READY_WP:
        if (!is_read)
        {
            m35fd_array[PCID].last_error = ERROR_PROTECTED;
            goto action_error;
        }

    case STATE_READY:
        if (disk_sector >= 1440)
        {
            m35fd_array[PCID].last_error = ERROR_BAD_SECTOR;
            goto action_error;
        }
    }

    tracks_delta = m35fd_array[PCID].current_track - disk_sector / 80;
    /* We assume sector seek time is 0 here. */
    do_cycle = *context.cycles_counter + *context.emu_freq / 60 +
        abs(tracks_delta) * *context.emu_freq * 24 / 10000;
    m35fd_array[PCID].event_ID =
        context.schedule_event(do_cycle, do_action, m35fd_array + PCID);
    m35fd_array[PCID].disk_sector = context.registers[3];
    m35fd_array[PCID].memory_location = context.registers[4];
    m35fd_array[PCID].is_read = is_read;
    m35fd_array[PCID].state = STATE_BUSY;

    context.registers[1] = 1;
    return;

  action_error:
    context.registers[1] = 0;
    if (m35fd_array[PCID].interrupt)
        context.send_int(m35fd_array[PCID].interrupt);
}

static unsigned int recv_int(unsigned short PCID)
{
    pthread_mutex_lock(&m35fd_array[PCID].lock);

    switch (context.registers[0])
    {
    case 0:
        context.registers[1] = m35fd_array[PCID].state;
        context.registers[2] = m35fd_array[PCID].last_error;
        m35fd_array[PCID].last_error = ERROR_NONE;
        break;

    case 1:
        m35fd_array[PCID].interrupt = context.registers[1];
        break;

    case 2:
        schedule_action(0, context.registers[3], context.registers[4], PCID);
        break;

    case 3:
        schedule_action(1, context.registers[3], context.registers[4], PCID);

    default:;
    }

    pthread_mutex_unlock(&m35fd_array[PCID].lock);
    return 0;
}

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[])
{
    unsigned int i;
    int result;
    context = *dddcpu16_context;

    if (argc > 1)
    {
        result = read_uint(argv[1]);
        if (result < 0)
        {
            printf("Could not read m35fd number \"%s\".\n", argv[1]);
            return 1;
        }
        if (!result)
        {
            printf("Cannot set m35fd number to 0.\n");
            return 1;
        }
        m35fd_number = result;
    }
    else
        m35fd_number = 1;

    m35fd_array = (struct m35fd_context*)
        malloc(sizeof(struct m35fd_context) * m35fd_number);
    for (i = 0; i < m35fd_number; ++i)
    {
        context.add_hard(info, recv_int, i);
        /* Other values are set when needed. */
        m35fd_array[i].state = STATE_NO_MEDIA;
        m35fd_array[i].last_error = ERROR_NONE;
        m35fd_array[i].interrupt = 0;
        m35fd_array[i].current_track = 0;
        m35fd_array[i].event_ID = 0;
        pthread_mutex_init(&m35fd_array[i].lock, NULL);
    }

    context.add_command("m35fd.insert", cmd_insert);
    context.add_command("m35fd.eject", cmd_eject);

    return 0;
}

void term(void)
{
    unsigned int i;
    if (m35fd_array)
    {
        for (i = 0; i < m35fd_number; ++i)
        {
            if (m35fd_array[i].event_ID)
                context.cancel_event(m35fd_array[i].event_ID, NULL);
            if (m35fd_array[i].state != STATE_NO_MEDIA)
                close(m35fd_array[i].floppy_fd);
        }
        free(m35fd_array);
    }
}
