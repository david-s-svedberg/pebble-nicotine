#include "config_menu_window_logic.h"

#include <pebble.h>

#include "edit_time_window.h"
#include "format.h"
#include "persistance.h"
#include "icons.h"
#include "scheduler.h"

static void handle_interval_edit(int index, void* context)
{
    setup_edit_time_window(Intervall);
}

static void handle_last_alarm_edit(int index, void* context)
{
    setup_edit_time_window(LastAlarm);
}

static void handle_next_alarm_edit(int index, void* context)
{
    setup_edit_time_window(NextAlarm);
}

void update_config_menu_items(SimpleMenuItem* menu_items)
{
    static char intervall_buf[6];
    static char last_alarm_buf[6];
    static char next_alarm_buf[6];
    Data* data = get_data();
    fill_time_string(intervall_buf, data->intervalMinutes / MINUTES_PER_HOUR, data->intervalMinutes % MINUTES_PER_HOUR);
    fill_time_string(last_alarm_buf, data->last_hour, data->last_minute);
    fill_time_string(last_alarm_buf, data->last_hour, data->last_minute);

    if(has_next_dose_time())
    {
        time_t next_wakup_time = get_next_wakup_time();
        struct tm* local_wakup_time = localtime(&next_wakup_time);
        fill_time_string(next_alarm_buf, local_wakup_time->tm_hour, local_wakup_time->tm_min);
    } else
    {
        snprintf(next_alarm_buf, 6, "None");
    }

    menu_items[0].title = "Intervall";
    menu_items[0].subtitle = intervall_buf;
    menu_items[0].callback = handle_interval_edit;
    menu_items[0].icon = NULL;

    menu_items[1].title = "Last Alarm Time";
    menu_items[1].subtitle = last_alarm_buf;
    menu_items[1].callback = handle_last_alarm_edit;
    menu_items[1].icon = NULL;

    menu_items[2].title = "Next Alarm Time";
    menu_items[2].subtitle = next_alarm_buf;
    menu_items[2].callback = handle_next_alarm_edit;
    menu_items[2].icon = NULL;
}