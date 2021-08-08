#pragma once

#include <stdint.h>
#include <pebble.h>


enum Setting
{
    NONE,
    Intervall,
    LastAlarm
};

typedef struct
{
    uint16_t intervalMinutes;
    WakeupId next_wakeup_id;
    uint8_t last_hour;
    uint8_t last_minute;
} Data;