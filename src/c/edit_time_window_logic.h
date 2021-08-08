#pragma once

#include <pebble.h>

#include "data.h"

void set_edit_time_layers(
    TextLayer *edit_time_hour_layer,
    TextLayer *edit_time_minute_layer,
    ActionBarLayer* edit_time_action_bar_layer
);
void update_edit_alarm_time_layers();
void set_edit_setting(enum Setting setting_to_edit);
void edit_alarm_action_bar_click_config_provider(void* context);
void change_to_init_edit_alarm_actions();