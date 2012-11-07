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

#include "console.h"

struct command* command_table = NULL;
unsigned int command_table_size = 0;

struct command_node* command_list;
unsigned int command_number = 0;

pthread_t console_main_thread = 0;

static unsigned int hash(const char* str)
{
    unsigned short r1 = 0;
    unsigned short r2 = 0;

    while (*str)
    {
        unsigned int chunk =
            *(unsigned short*)str | *(unsigned short*)str << 16;
        r1 = r1 ^ chunk >> (r2 & 0xf);
        r2 = r2 ^ chunk >> (r1 & 0xf);
        if (!*(str + 1))
            break;
        str += 2;
    }

    return r1 ^ r2;
}

static void* console_main(void* arguments)
{
    while (emu_run)
    {
        char buffer[COMMAND_BUFFER_SIZE];
        ssize_t read_length;
        struct uint_node* offset_list = NULL;
        struct uint_node* offset_node_temp;
        unsigned int argc = 0;
        const char** argv;
        unsigned int i = 0;

        printf("(dddcpu16) ");
        fflush(stdout);
        do
            read_length = read(STDIN_FILENO, buffer, COMMAND_BUFFER_SIZE);
        while (!read_length);
        /* Replace trailing '\n' by a '\0' terminator
           and update read_length accordingly. */
        buffer[--read_length] = '\0';

        /* Split command line against spaces. */
        for (i = 0; i < read_length; ++i)
        {
            while (buffer[i] == ' ')
                ++i;
            if (!buffer[i])
                break;

            offset_node_temp =
                (struct uint_node*)malloc(sizeof(struct uint_node));
            offset_node_temp->value = i;
            offset_node_temp->next = offset_list;
            offset_list = offset_node_temp;
            ++argc;
            while (buffer[i] != ' ' && buffer[i])
                ++i;
            buffer[i] = '\0';
        }
        if (!argc)
            continue;

        argv = (const char**)malloc(sizeof(const char*) * argc);
        i = argc;
        while (i--)
        {
            argv[i] = buffer + offset_list->value;
            offset_node_temp = offset_list;
            offset_list = offset_list->next;
            free(offset_node_temp);
        }

        for (i = hash(argv[0]) % command_table_size;;
             i = (i + 1) % command_table_size)
        {
            if (!command_table[i].name)
            {
                printf("Unknown command: %s.\n", argv[0]);
                break;
            }
            if (!strcmp(argv[0], command_table[i].name))
            {
                command_table[i].callback(argc, argv);
                break;
            }
        }

        free(argv);
    }

    return NULL;
}

void add_command(const char* name,
                 void (* callback)(unsigned int, const char*[]))
{
    struct command_node* command_node_temp =
        (struct command_node*)malloc(sizeof(struct command_node));

    command_node_temp->command.name = strdup(name);
    command_node_temp->command.callback = callback;
    command_node_temp->next = command_list;
    command_list = command_node_temp;
    ++command_number;
}

void init_console(void)
{
    unsigned int i;

    /* To operate correctly (i.e. O(1) amortized), a hash table's load factor
       should not exceed 70%. We do not need optimal efficiency to handle
       commands since the console runs in a differant thread from the emulation,
       but a lot of collisions tend to happen when the load factor approches
       100%. A load factor of 80% sounds to be good. Also, we need at least an
       empty space to detect unknown commands. */
    command_table_size = command_number * 5 / 4 + 1;
    command_table =
        (struct command*)malloc(command_table_size * sizeof(struct command));

    for (i = 0; i < command_table_size; ++i)
        command_table[i].name = NULL;

    while (command_list)
    {
        struct command_node* command_node_temp = command_list;
        unsigned short index =
            hash(command_list->command.name) % command_table_size;

        while (command_table[index].name)
            index = (index + 1) % command_table_size;
        command_table[index] = command_list->command;

        command_list = command_list->next;
        free(command_node_temp);
    }

    pthread_create(&console_main_thread, NULL, console_main, NULL);
}

void term_console(void)
{
    if (command_table)
    {
        unsigned int i;
        for (i = 0; i < command_table_size; ++i)
            if (command_table[i].name)
                free((void*)command_table[i].name);
        free(command_table);
    }
    else
        while (command_list)
        {
            struct command_node* command_node_temp = command_list;
            command_list = command_list->next;
            free((void*)command_node_temp->command.name);
            free(command_node_temp);
        }
    pthread_cancel(console_main_thread);
    pthread_join(console_main_thread, NULL);
}
