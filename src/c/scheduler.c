#include "scheduler.h"

#include <pebble.h>

#include "persistance.h"

static void schedule(Data* data, time_t time)
{
    struct tm* local_scheduled_time = localtime(&time);
    APP_LOG(
        APP_LOG_LEVEL_DEBUG,
        "scheduling alarm for '%02d:%02d'",
        local_scheduled_time->tm_hour,
        local_scheduled_time->tm_min);

    WakeupId id = -1;
    do {
        id = wakeup_schedule(time, 0, true);
        if(id == E_RANGE)
        {
            time += SECONDS_PER_MINUTE / 2;
        }
    } while(id == E_RANGE);

    if(id >= 0)
    {
        data->next_wakeup_id = id;
    } else
    {
        switch (id)
        {
        case E_INVALID_ARGUMENT:
            APP_LOG(APP_LOG_LEVEL_ERROR, "ERROR: Time is in the past");
            break;
        case E_OUT_OF_RESOURCES:
            APP_LOG(APP_LOG_LEVEL_ERROR, "ERROR: No more wakups for this app");
            break;
        case E_INTERNAL:
            APP_LOG(APP_LOG_LEVEL_ERROR, "ERROR: system error occurred during scheduling");
            break;
        default:
            break;
        }
    }
}

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
        schedule(data, wakup_time);
    }
}

void schedule_alarm(time_t t)
{
    schedule(get_data(), t);
}

char* get_next_dose_time_string()
{
    static char next_dose_buffer[6];
    fill_next_dose_time(next_dose_buffer);

    return next_dose_buffer;
}