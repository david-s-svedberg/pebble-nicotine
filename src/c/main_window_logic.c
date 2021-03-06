#include "main_window_logic.h"

#include "config_menu_window.h"
#include "persistance.h"
#include "format.h"
#include "scheduler.h"

static TextLayer* m_next_dose_layer;

void take_next_dose(ClickRecognizerRef recognizer, void* context)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Take next dose requested");
    vibes_long_pulse();
    unschedule_all();
    schedule_next_alarm();
    save_data();
    update_next_dose_time();
}

void goto_config_window(ClickRecognizerRef recognizer, void* context)
{
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Goto Config requested");
    setup_config_menu_window();
}

void update_next_dose_time()
{
    static char next_dose_buffer[6];
    fill_next_dose_time(next_dose_buffer);
    text_layer_set_text(m_next_dose_layer, next_dose_buffer);
}

void set_main_window_layers(TextLayer* next_dose_layer)
{
    m_next_dose_layer = next_dose_layer;
}