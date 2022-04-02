#include "config_menu_window_logic.h"

#include <pebble.h>

#include "edit_time_window.h"
#include "format.h"
#include "persistance.h"
#include "icons.h"
#include "scheduler.h"

char* get_timeout_text()
{
    static char timeout_buffer[10];

    uint8_t timeout_min = get_alarm_timeout();
    snprintf(timeout_buffer, 10, "%d minutes", timeout_min);

    return timeout_buffer;
}

static uint8_t get_next_timeout()
{
    uint8_t next = get_alarm_timeout() + 1;
    if(next > 5)
    {
        next = 1;
    }

    return next;
}

void handle_timeout_edit()
{
    set_alarm_timeout(get_next_timeout());
}

void handle_interval_edit(int index, void* context)
{
    setup_edit_time_window(Intervall);
}

void handle_last_alarm_edit(int index, void* context)
{
    setup_edit_time_window(LastAlarm);
}

void handle_next_alarm_edit(int index, void* context)
{
    setup_edit_time_window(NextAlarm);
}

char* get_last_alarm_text()
{
    static char last_alarm_buf[6];

    Data* data = get_data();
    fill_time_string(last_alarm_buf, data->last_hour, data->last_minute);

    return last_alarm_buf;
}

char* get_next_alarm_text()
{
   static char next_alarm_buf[6];

    if(has_next_dose_time())
    {
        time_t next_wakup_time = get_next_wakup_time();
        struct tm* local_wakup_time = localtime(&next_wakup_time);
        fill_time_string(next_alarm_buf, local_wakup_time->tm_hour, local_wakup_time->tm_min);
    } else
    {
        snprintf(next_alarm_buf, 6, "None");
    }

    return next_alarm_buf;
}

char* get_interval_text()
{
    static char intervall_buf[6];

    Data* data = get_data();
    fill_time_string(intervall_buf, data->intervalMinutes / MINUTES_PER_HOUR, data->intervalMinutes % MINUTES_PER_HOUR);

    return intervall_buf;
}

void update_interval_item(SimpleMenuItem* interval_item)
{
    interval_item->subtitle = get_interval_text();
}

void update_next_alarm_item(SimpleMenuItem* next_alarm_item)
{
    next_alarm_item->subtitle = get_next_alarm_text();
}

void update_last_alarm_item(SimpleMenuItem* last_alarm_item)
{
    last_alarm_item->subtitle = get_last_alarm_text();
}

void update_timeout_item(SimpleMenuItem* timeout_item)
{
    timeout_item->subtitle = get_timeout_text();
}