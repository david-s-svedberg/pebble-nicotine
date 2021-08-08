#include "app.h"

#include "main_window.h"
#include "config_menu_window.h"
#include "edit_time_window.h"
#include "alarm_window.h"

#include "icons.h"
#include "app_glance.h"
#include "persistance.h"

static void wakeup_handler(WakeupId id, int32_t alarm_index)
{
    setup_alarm_window();
}

static bool first_start()
{
    return !has_any_data();
}

void init()
{
    if(first_start())
    {
        APP_LOG(APP_LOG_LEVEL_INFO, "Removing old wake ups");
        // Mostly for development
        // But old wake ups are not removed when reinstalling app
        // So we remove them so no collisions happen
        wakeup_cancel_all();
    }
    if(launch_reason() == APP_LAUNCH_WAKEUP)
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting from Wakeup");

        setup_alarm_window();
    } else
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Starting from non Wakeup");

        wakeup_service_subscribe(wakeup_handler);

        setup_main_window();
    }
}

void deinit()
{
    APP_LOG(APP_LOG_LEVEL_INFO, "Deiniting Dose");

    tear_down_main_window();
    tear_down_config_menu_window();
    tear_down_edit_time_window();
    tear_down_alarm_window();
    destroy_all_icons();
    setup_app_glance();
}