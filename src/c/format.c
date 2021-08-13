#include "format.h"

#include <stdio.h>

void fill_time_unit_string(char* buf, uint8_t timeUnit)
{
    snprintf(buf, 3, "%02d", timeUnit);
}

void fill_time_string(char* buf, uint8_t hour, uint8_t minute)
{
    snprintf(buf, 6, "%02d:%02d", hour, minute);
}