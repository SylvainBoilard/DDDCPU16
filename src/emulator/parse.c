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

#include "parse.h"

long read_hz(const char* string)
{
    unsigned long result = 0;

    if (*string < '0' || *string > '9')
        return -1;

    do
        result = result * 10 + *string - '0';
    while (*++string >= '0' && *string <= '9');

    if (!*string)
        return result;
    switch (*string)
    {
    case 'G':
        result *= 1000000000;
        ++string;
        break;

    case 'M':
        result *= 1000000;
        ++string;
        break;

    case 'k':
        result *= 1000;
        ++string;

    default:;
    }
    if (*string == 'H' && *++string == 'z' && !*++string)
        return result;
    return -1;
}

long read_ns(const char* string)
{
    unsigned long result = 0;

    if (*string < '0' || *string > '9')
        return -1;

    do
        result = result * 10 + *string - '0';
    while (*++string >= '0' && *string <= '9');

    if (!*string)
        return result;
    switch (*string)
    {
    case 'm':
        result *= 1000000;
        break;

    case 'u':
        result *= 1000;
        break;

    case 'n':
        break;

    default:
        return -1;
    }
    if (*++string == 's' && !*++string)
        return result;
    return -1;
}

long read_float_10E3(const char* string)
{
    unsigned long result = 0;

    if (*string < '0' || *string > '9')
        return -1;

    do
        result = result * 10 + *string - '0';
    while (*++string >= '0' && *string <= '9');

    if (!*string)
        return result * 1000;
    if (*string != '.')
        return -1;
    ++string;

    if (*string < '0' || *string > '9')
        return -1;
    result = result * 10 + *string - '0';
    ++string;

    if (!*string)
        return result * 100;
    if (*string < '0' || *string > '9')
        return -1;
    result = result * 10 + *string - '0';
    ++string;

    if (!*string)
        return result * 10;
    if (*string < '0' || *string > '9')
        return -1;
    result = result * 10 + *string - '0';
    ++string;

    if (!*string)
        return result;
    return -1;
}
