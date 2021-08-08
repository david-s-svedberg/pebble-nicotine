#include "scheduler.h"

#include <pebble.h>

#include "persistance.h"

void unschedule_all()
{
    wakeup_cancel_all();
}

void schedule_next_alarm()
{
    Data* data = get_data();
    time_t wakup_time = time(NULL) + (data->intervalMinutes * SECONDS_PER_MINUTE);
    time_t latest_alarm = clock_to_timestamp(TODAY, data->last_hour, data->last_minute);

    if(wakup_time < latest_alarm)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "scheduling next wakeup");
        data->next_wakeup_id = wakeup_schedule(wakup_time, 0, true);
    }
}

void schedule_snooze_alarm(time_t t)
{
    Data* data = get_data();
    data->next_wakeup_id = wakeup_schedule(t, 0, true);
}