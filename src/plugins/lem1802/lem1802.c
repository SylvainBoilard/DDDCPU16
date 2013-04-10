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

#include "lem1802.h"

struct dddcpu16_context context;
unsigned int lem1802_number = 1;
struct lem1802_context* lem1802_array = NULL;
unsigned int lem1802_ratio = 1;
unsigned int lem1802_fps = 10;

pthread_t display_thread;

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

static void display_lem1802(unsigned int n)
{
    const unsigned short* font;
    const unsigned short* palette;
    unsigned short border_color;
    unsigned char i;
    unsigned char j;

    sfWindow_setActive(lem1802_array[n].window, 1);

    if (!lem1802_array[n].screen_map ||
        ++lem1802_array[n].heating < lem1802_fps)
    {
        float val = (float)lem1802_array[n].heating / lem1802_fps;
        glClearColor(val, val, val, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        sfWindow_display(lem1802_array[n].window);
        return;
    }

    font = lem1802_array[n].font_map ?
        context.memory + lem1802_array[n].font_map : default_font;
    palette = lem1802_array[n].palette_map ?
        context.memory + lem1802_array[n].palette_map : default_palette;
    border_color = palette[lem1802_array[n].border_color];

    glClearColor((border_color >> 4 & 0xf0) / 16.f,
                 (border_color & 0xf0) / 16.f,
                 (border_color << 4 & 0xf0) / 16.f,
                 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < 12; ++i)
        for (j = 0; j < 32; ++j)
        {
            /* TODO:
               - Prevent overflows.
               - Handle endianness.
               - Support scaling.
               - Get colors to work. */
            const unsigned short current_char =
                context.memory[lem1802_array[n].screen_map + i * 32 + j];
            const unsigned short colors[2] = {
                palette[current_char >> 8 & 0xf], palette[current_char >> 12]
            };
            unsigned int bits = 0;

            glColorTable(GL_COLOR_TABLE, GL_RGB4, 2, GL_RGB,
                         GL_UNSIGNED_SHORT_4_4_4_4, colors);
            glRasterPos2s(12 + 8 * i, 8 + 4 * j);
            if (!(current_char >> 7 & 1) ||
                (lem1802_array[n].heating / lem1802_fps) % 2)
            {
                bits = font[(current_char & 0x7f) * 2] << 16 |
                    font[(current_char & 0x7f) * 2 + 1];
            }
            glBitmap(8, 4, 0.f, 0.f, 0.f, 0.f, (const GLubyte*)&bits);
        }

    sfWindow_display(lem1802_array[n].window);
}

static void* lem1802_run(void* argument)
{
    while (1)
    {
        unsigned int i;

        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

        for (i = 0; i < lem1802_number; ++i)
            display_lem1802(i);

        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
    }

    return NULL; /* This function never actually returns. */
}

static unsigned int recv_int(unsigned short PCID)
{
    unsigned int i;

    switch (context.registers[0])
    {
    case 0:
        lem1802_array[PCID].screen_map = context.registers[1];
        if (!lem1802_array[PCID].screen_map)
            lem1802_array[PCID].heating = 0;
        break;

    case 1:
        lem1802_array[PCID].font_map = context.registers[1];
        break;

    case 2:
        lem1802_array[PCID].palette_map = context.registers[1];
        break;

    case 3:
        lem1802_array[PCID].border_color = context.registers[1];
        break;

    case 4:
        for (i = 0; i < 256; ++i)
            context.memory[(context.registers[1] + i) % 0x10000] =
                default_font[i];
        return 256;

    case 5:
        for (i = 0; i < 16; ++i)
            context.memory[(context.registers[1] + i) % 0x10000] =
                default_palette[i];
        return 16;

    default:;
    }

    return 0;
}

static void info(void)
{
    context.registers[0] = 0xf615;
    context.registers[1] = 0x7349;
    context.registers[2] = 0x1802;
    context.registers[3] = 0x8b36;
    context.registers[4] = 0x1c6c;
}

int init(const struct dddcpu16_context* dddcpu16_context,
         int argc, char* argv[])
{
    int i;
    unsigned int j;
    sfVideoMode videomode;
    context = *dddcpu16_context;

    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            int result;

            switch (argv[i][1])
            {
            case 'f': /* Set lem1802 refresh rate. */
                if (++i >= argc)
                {
                    printf("You need to precise a refresh "
                           "rate number with option -f.\n");
                    return 1;
                }
                result = read_uint(argv[i]);

                if (result < 0)
                {
                    printf("Error reading refresh rate %s.\n", argv[i]);
                    return 1;
                }
                if (!result)
                {
                    printf("Cannot set refresh rate to 0.\n");
                    return 1;
                }

                lem1802_fps = result;
                break;

            case 'n': /* Set number of lem1802 to connect. */
                if (++i >= argc)
                {
                    printf("You need to precise a lem1802 "
                           "number with option -n.\n");
                    return 1;
                }
                result = read_uint(argv[i]);

                if (result < 0)
                {
                    printf("Error reading lem1802 number %s.\n", argv[i]);
                    return 1;
                }
                if (!result)
                {
                    printf("Cannot set lem1802 number to 0.\n");
                    return 1;
                }

                lem1802_number = result;
                break;

            case 'r': /* Set lem1802 display ratio. */
                if (++i >= argc)
                {
                    printf("You need to precise a positive "
                           "integer with option -r.\n");
                    return 1;
                }
                result = read_uint(argv[i]);

                if (result < 0)
                {
                    printf("Error reading ratio %s.\n", argv[i]);
                    return 1;
                }
                if (!result)
                {
                    printf("Cannot set ratio to 0.\n");
                    return 1;
                }

                lem1802_ratio = result;
                break;

            default:
                printf("Unknown option: %s.\n", argv[i]);
                return 1;
            }
        }
        else
        {
            printf("Unknown option: %s.\n", argv[i]);
            return 1;
        }
    }

    lem1802_array = (struct lem1802_context*)
        malloc(sizeof(struct lem1802_context) * lem1802_number);
    videomode.width = lem1802_ratio * 104;
    videomode.height = lem1802_ratio * 136;
    videomode.bitsPerPixel = 12;
    for (j = 0; j < lem1802_number; ++j)
    {
        context.add_hard(info, recv_int, j);
        lem1802_array[j].window = /* TODO: Handle failing to create a window. */
            sfWindow_create(videomode, "LEM1802", sfTitlebar, NULL);
        sfWindow_setActive(lem1802_array[j].window, 0);
        glOrtho(104, 0, 136, 0, 1, -1);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        lem1802_array[j].screen_map = 0;
        lem1802_array[j].font_map = 0;
        lem1802_array[j].palette_map = 0;
        lem1802_array[j].border_color = 0;
        lem1802_array[j].heating = 0;
        sfWindow_setFramerateLimit(lem1802_array[j].window, lem1802_fps);
    }

    pthread_create(&display_thread, NULL, lem1802_run, NULL);

    return 0;
}

void term(void)
{
    if (lem1802_array)
    {
        unsigned int i;

        pthread_cancel(display_thread);
        pthread_join(display_thread, NULL);

        for (i = 0; i < lem1802_number; ++i)
            sfWindow_destroy(lem1802_array[i].window);
        free(lem1802_array);
    }
}
