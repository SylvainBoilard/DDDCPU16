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

#include "hardware.h"

struct hardware* hd_hard = NULL;
unsigned int hd_number = 0;

struct hardware_node
{
    struct hardware_node* next;
    struct hardware hard;
};

static struct hardware_node* hd_list = NULL;

void recv_int(unsigned short int_val)
{
    static pthread_mutex_t iq_front_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&iq_front_mutex);
    int_queue[iq_front] = int_val;
    ++iq_front; /* Do not increment iq_front until queuing complete. */
    pthread_mutex_unlock(&iq_front_mutex);
}

int load_hard(int hard_argc, char* hard_argv[])
{
    static const struct dddcpu16_context context = {
	memory, registers, &emu_freq, &emu_speed, &cycles_counter, recv_int
    };
    struct hardware_node* hard_node_tmp;
    void* dl_handle;
    void* hd_info;
    void* hd_send_int;
    int (* hd_init)(const struct dddcpu16_context*, int, char*[]);

    if (!hard_argc)
    {
	printf("You have to specify a hardware library with option -h.\n");
	return 1;
    }
    dl_handle = dlopen(hard_argv[0], RTLD_LAZY);
    if (!dl_handle)
    {
	printf("%s\n", dlerror());
	return 2;
    }

    hd_info = dlsym(dl_handle, "info");
    if (!hd_info)
	printf("%s\n", dlerror());
    hd_send_int = dlsym(dl_handle, "recv_int");
    if (!hd_send_int)
	printf("%s\n", dlerror());
    /* C9X leaves cast from void* to function pointer undefined.
       The assignement used below is the POSIX.1-2003 TC1 workaround. */
    *(void**)(&hd_init) = dlsym(dl_handle, "init");
    if (!hd_init)
	printf("%s\n", dlerror());
    if (!(hd_init && hd_info && hd_send_int))
    {
	dlclose(dl_handle);
	return 2;
    }

    /* Hardware is correctly loaded, we can push it. */
    hard_node_tmp = (struct hardware_node*)malloc(sizeof(struct hardware_node));
    *(void**)(&hard_node_tmp->hard.hd_info) = hd_info;
    *(void**)(&hard_node_tmp->hard.hd_send_int) = hd_send_int;
    hard_node_tmp->hard.dl_handle = dl_handle;
    hard_node_tmp->next = hd_list;
    hd_list = hard_node_tmp;
    ++hd_number;

    return hd_init(&context, hard_argc, hard_argv);
}

void complete_load_hard(void)
{
    unsigned int i;
    hd_hard = (struct hardware*)malloc(sizeof(struct hardware) * hd_number);
    for (i = 0; i < hd_number; ++i)
    {
	struct hardware_node* hard_node = hd_list;
	hd_list = hard_node->next;
	hd_hard[i] = hard_node->hard;
	free(hard_node);
    }
}

void free_hard(void)
{
    void (* hd_term)(void);

    if (hd_hard)
    {
	unsigned int i;
	for (i = 0; i < hd_number; ++i)
	{
	    *(void**)&hd_term = dlsym(hd_hard[i].dl_handle, "term");
	    if (hd_term)
		hd_term();
	    dlclose(hd_hard[i].dl_handle);
	}
	free(hd_hard);
	hd_hard = NULL;
    }
    else
	while (hd_list)
	{
	    struct hardware_node* hard_node = hd_list;
	    hd_list = hard_node->next;

	    *(void**)&hd_term = dlsym(hard_node->hard.dl_handle, "term");
	    if (hd_term)
		hd_term();
	    dlclose(hard_node->hard.dl_handle);
	    free(hard_node);
	}
}
