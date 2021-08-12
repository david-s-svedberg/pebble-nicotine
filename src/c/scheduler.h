#pragma once

#include "data.h"

void unschedule_all();
void schedule_next_alarm();
void schedule_alarm(time_t t);