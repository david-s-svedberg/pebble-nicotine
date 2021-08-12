#include "edit_time_window_logic.h"

#include <pebble.h>

#include "persistance.h"
#include "format.h"
#include "icons.h"
#include "scheduler.h"
#include "data.h"

static TextLayer *m_edit_time_hour_layer;
static TextLayer *m_edit_time_minute_layer;

static ActionBarLayer* m_edit_time_action_bar_layer;

enum Setting m_edit_setting = NONE;
uint8_t m_hour = 0;
uint8_t m_minute = 0;
static short m_current_edit_step = 0;

static void save_time_and_go_back(ClickRecognizerRef recognizer, void* context)
{
    Data* data = get_data();
    switch (m_edit_setting)
    {
        case Intervall:
            data->intervalMinutes = (m_hour * MINUTES_PER_HOUR) + m_minute;
            break;
        case LastAlarm:
            data->last_hour = m_hour;
            data->last_minute = m_minute;
            break;
        case NextAlarm:
            unschedule_all();
            time_t t = clock_to_timestamp(TODAY, m_hour, m_minute);
            schedule_alarm(t);
            break;
        default:
            APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown Setting");
    }
    /**
     * Could reschedule active alarm here
    **/
    save_data();
    window_stack_pop(true);
}

static void highlight_hours()
{
    text_layer_set_background_color(m_edit_time_hour_layer, GColorWhite);
    text_layer_set_text_color(m_edit_time_hour_layer, GColorBlack);
}

static void un_highlight_hours()
{
    text_layer_set_background_color(m_edit_time_hour_layer, GColorBlack);
    text_layer_set_text_color(m_edit_time_hour_layer, GColorWhite);
}

static void highlight_minutes()
{
    text_layer_set_background_color(m_edit_time_minute_layer, GColorWhite);
    text_layer_set_text_color(m_edit_time_minute_layer, GColorBlack);
}

static void un_highlight_minutes()
{
    text_layer_set_background_color(m_edit_time_minute_layer, GColorBlack);
    text_layer_set_text_color(m_edit_time_minute_layer, GColorWhite);
}

static void increase_time(ClickRecognizerRef recognizer, void* context)
{
    if(m_current_edit_step == 0)
    {
        if(m_hour < 23)
        {
            m_hour++;
        } else
        {
            m_hour = 0;
        }

    } else if(m_current_edit_step == 1)
    {
        if(m_minute < 59)
        {
            m_minute++;
        } else
        {
            m_minute = 0;
        }
    }
    update_edit_alarm_time_layers();
}

static void decrease_time(ClickRecognizerRef recognizer, void* context)
{
    if(m_current_edit_step == 0)
    {
        if(m_hour > 0)
        {
            m_hour--;
        } else
        {
            m_hour = 23;
        }
    } else if(m_current_edit_step == 1)
    {
        if(m_minute > 0)
        {
            m_minute--;
        } else
        {
            m_minute = 59;
        }
    }
    update_edit_alarm_time_layers();
}

static void goto_next_edit(ClickRecognizerRef recognizer, void* context)
{
    m_current_edit_step++;
    if(m_current_edit_step > 1)
    {
        change_to_init_edit_alarm_actions();
        un_highlight_minutes();
    } else if(m_current_edit_step == 1)
    {
        un_highlight_hours();
        highlight_minutes();
    }
}

static void goto_previous_edit(ClickRecognizerRef recognizer, void* context)
{
    m_current_edit_step--;
    if(m_current_edit_step < 0)
    {
        change_to_init_edit_alarm_actions();
        un_highlight_hours();
    } else if(m_current_edit_step == 0)
    {
        highlight_hours();
        un_highlight_minutes();
    }
}

static void edit_time_action_bar_click_config_provider(void* context)
{
    window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, increase_time);
    window_single_click_subscribe(BUTTON_ID_SELECT, goto_next_edit);
    window_single_click_subscribe(BUTTON_ID_BACK, goto_previous_edit);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, decrease_time);
}

static void edit_time(ClickRecognizerRef recognizer, void* context)
{
    action_bar_layer_set_click_config_provider(m_edit_time_action_bar_layer, edit_time_action_bar_click_config_provider);
    action_bar_layer_set_icon_animated(m_edit_time_action_bar_layer, BUTTON_ID_UP, get_up_icon(), true);
    action_bar_layer_set_icon_animated(m_edit_time_action_bar_layer, BUTTON_ID_SELECT, get_play_icon(), true);
    action_bar_layer_set_icon_animated(m_edit_time_action_bar_layer, BUTTON_ID_DOWN, get_down_icon(), true);
    m_current_edit_step = 0;

    highlight_hours();
}

void change_to_init_edit_alarm_actions()
{
    action_bar_layer_set_click_config_provider(m_edit_time_action_bar_layer, edit_alarm_action_bar_click_config_provider);

    action_bar_layer_clear_icon(m_edit_time_action_bar_layer, BUTTON_ID_UP);
    action_bar_layer_set_icon_animated(m_edit_time_action_bar_layer, BUTTON_ID_SELECT, get_edit_icon(), true);
    action_bar_layer_set_icon_animated(m_edit_time_action_bar_layer, BUTTON_ID_DOWN, get_check_icon(), true);
}


void edit_alarm_action_bar_click_config_provider(void* context)
{
    window_single_click_subscribe(BUTTON_ID_SELECT, edit_time);
    window_single_click_subscribe(BUTTON_ID_DOWN, save_time_and_go_back);
}

void update_edit_alarm_time_layers()
{
    static char hour_buffer[3];
    static char minute_buffer[3];
    fill_time_unit_string(hour_buffer, m_hour);
    fill_time_unit_string(minute_buffer, m_minute);

    text_layer_set_text(m_edit_time_hour_layer, hour_buffer);
    text_layer_set_text(m_edit_time_minute_layer, minute_buffer);
}

void set_edit_time_layers(
    TextLayer *edit_time_hour_layer,
    TextLayer *edit_time_minute_layer,
    ActionBarLayer* edit_time_action_bar_layer
)
{
    m_edit_time_hour_layer = edit_time_hour_layer;
    m_edit_time_minute_layer = edit_time_minute_layer;
    m_edit_time_action_bar_layer = edit_time_action_bar_layer;
}

void set_edit_setting(enum Setting setting_to_edit)
{
    m_edit_setting = setting_to_edit;
    Data* data = get_data();
    switch (m_edit_setting)
    {
        case Intervall:
            m_hour = data->intervalMinutes / MINUTES_PER_HOUR;
            m_minute = data->intervalMinutes % MINUTES_PER_HOUR;
            break;
        case LastAlarm:
            m_hour = data->last_hour;
            m_minute = data->last_minute;
            break;
        case NextAlarm:
            m_hour = 0;
            m_minute = 0;
            if(has_next_dose_time())
            {
                time_t next_wakup_time = get_next_wakup_time();
                struct tm* local_wakup_time = localtime(&next_wakup_time);
                m_hour = local_wakup_time->tm_hour;
                m_minute = local_wakup_time->tm_min;
            }
            break;
        default:
            APP_LOG(APP_LOG_LEVEL_ERROR, "Unknown Setting");
    }
}