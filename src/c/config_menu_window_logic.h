#pragma once

#include <pebble.h>

void reset_alarms(int index, void* context);

void handle_interval_edit(int index, void* context);
void handle_last_alarm_edit(int index, void* context);
void handle_next_alarm_edit(int index, void* context);
void handle_timeout_edit();

void update_interval_item(SimpleMenuItem* interval_item);
void update_next_alarm_item(SimpleMenuItem* next_alarm_item);
void update_last_alarm_item(SimpleMenuItem* last_alarm_item);
void update_timeout_item(SimpleMenuItem* timeout_item);

char* get_last_alarm_text();
char* get_next_alarm_text();
char* get_interval_text();
char* get_timeout_text();