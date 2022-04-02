#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "data.h"

Data* get_data();

uint16_t get_minutes_until_next_dose();
time_t get_next_wakup_time();
bool has_next_dose_time();

bool has_any_data();
void save_data();
void fill_next_dose_time(char* buf);

uint8_t get_alarm_timeout();
void set_alarm_timeout(uint8_t min);