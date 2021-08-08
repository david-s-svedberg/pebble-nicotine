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

void update_config_menu_items(SimpleMenuItem* menu_items)
{
    static char intervall_buf[6];
    static char last_alarm_buf[6];
    Data* data = get_data();
    fill_time_string(intervall_buf, data->intervalMinutes / MINUTES_PER_HOUR, data->intervalMinutes % MINUTES_PER_HOUR);
    fill_time_string(last_alarm_buf, data->last_hour, data->last_minute);

    menu_items[0].title = "Intervall";
    menu_items[0].subtitle = intervall_buf;
    menu_items[0].callback = handle_interval_edit;
    menu_items[0].icon = NULL;

    menu_items[1].title = "Last Alarm Time";
    menu_items[1].subtitle = last_alarm_buf;
    menu_items[1].callback = handle_last_alarm_edit;
    menu_items[1].icon = NULL;
}