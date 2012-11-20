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

#include "plugin.h"

struct plugin_node* plugin_list = NULL;
struct plugin_args_node* plugin_args_list = NULL;

void add_plugin(int plugin_argc, char* plugin_argv[])
{
    struct plugin_args_node* plugin_args_node_temp =
        (struct plugin_args_node*)malloc(sizeof(struct plugin_args_node));
    plugin_args_node_temp->next = plugin_args_list;
    plugin_args_node_temp->argc = plugin_argc;
    plugin_args_node_temp->argv = plugin_argv;
    plugin_args_list = plugin_args_node_temp;
}

int load_plugins(void)
{
    const struct dddcpu16_context context = {
        /* Variables */
        memory, registers, &emu_freq, &emu_speed, &emu_granularity,
        &cycles_counter,
        /* Functions */
        add_hard, add_command, recv_int, schedule_event, cancel_event
    };

    while (plugin_args_list)
    {
        struct plugin_node* plugin_node_temp;
        struct plugin_args_node* plugin_args_node_temp;
        void* dl_handle;
        int (* plugin_init)(const struct dddcpu16_context*, int, char*[]);
        int ret_val;

        /* RTLD_NOLOAD seems to be a glibc-only flag,
           this could cause portability issues. */
        if (dlopen(plugin_args_list->argv[0], RTLD_NOLOAD | RTLD_LAZY))
        {
            printf("Error loading %s : plugins can only be loaded once.\n",
                   plugin_args_list->argv[0]);
            return 1;
        }

        dl_handle = dlopen(plugin_args_list->argv[0], RTLD_LAZY);
        if (!dl_handle)
        {
            printf("%s\n", dlerror());
            return 2;
        }

        /* C9X leaves cast from void* to function pointer undefined.
           The assignement used below is the POSIX.1-2003 TC1 workaround. */
        *(void**)(&plugin_init) = dlsym(dl_handle, "init");
        if (!plugin_init)
        {
            printf("%s\n", dlerror());
            dlclose(dl_handle);
            return 2;
        }

        /* Plugin is correctly loaded, we can push it. */
        plugin_node_temp =
            (struct plugin_node*)malloc(sizeof(struct plugin_node));
        plugin_node_temp->dl_handle = dl_handle;
        plugin_node_temp->next = plugin_list;
        plugin_list = plugin_node_temp;

        ret_val = plugin_init(&context, plugin_args_list->argc,
                              plugin_args_list->argv);
        if (ret_val)
            return ret_val;

        plugin_args_node_temp = plugin_args_list;
        plugin_args_list = plugin_args_list->next;
        free(plugin_args_node_temp);
    }

    return 0;
}

void free_plugins(void)
{
    while (plugin_args_list)
    {
        struct plugin_args_node* current_node = plugin_args_list;
        plugin_args_list = plugin_args_list->next;
        free(current_node);
    }
    while (plugin_list)
    {
        void (* plugin_term)(void);
        struct plugin_node* current_node = plugin_list;
        plugin_list = plugin_list->next;

        *(void**)&plugin_term = dlsym(current_node->dl_handle, "term");
        if (plugin_term)
            plugin_term();
        dlclose(current_node->dl_handle);
        free(current_node);
    }
}
