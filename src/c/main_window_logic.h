#pragma once

#include <pebble.h>

void take_next_dose(ClickRecognizerRef recognizer, void* context);
void goto_config_window(ClickRecognizerRef recognizer, void* context);
void update_next_dose_time();
void set_main_window_layers(TextLayer* next_alarm_layer);