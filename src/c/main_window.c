#include "main_window.h"

#include <pebble.h>

#include "main_window_logic.h"
#include "icons.h"

static Window *main_window;

static StatusBarLayer* status_bar;

static TextLayer* title_layer;
static TextLayer* next_dose_label_layer;
static TextLayer* next_dose_layer;

static ActionBarLayer* main_window_action_bar_layer;

static void main_window_click_config_provider(void* context)
{
    window_single_click_subscribe(BUTTON_ID_DOWN, goto_config_window);
    window_long_click_subscribe(BUTTON_ID_SELECT, 500, take_next_dose, NULL);
}

static void setup_main_window_action_bar_layer(Layer *window_layer, GRect bounds)
{
    main_window_action_bar_layer = action_bar_layer_create();
    action_bar_layer_set_background_color(main_window_action_bar_layer, GColorWhite);
    action_bar_layer_add_to_window(main_window_action_bar_layer, main_window);
    action_bar_layer_set_click_config_provider(main_window_action_bar_layer, main_window_click_config_provider);

    action_bar_layer_set_icon_animated(main_window_action_bar_layer, BUTTON_ID_SELECT, get_check_icon(), true);
    action_bar_layer_set_icon_animated(main_window_action_bar_layer, BUTTON_ID_DOWN, get_config_icon(), true);
}

void on_time_tick(struct tm *tick_time, TimeUnits units_changed)
{
    update_next_dose_time();
}

static void update_main_window(Window *window)
{
    update_next_dose_time();
    tick_timer_service_subscribe(MINUTE_UNIT, on_time_tick);
}

static void disable_main_window(Window *window)
{
    tick_timer_service_unsubscribe();
}

static void setup_next_dose_layer(Layer *window_layer, GRect bounds)
{
    next_dose_label_layer = text_layer_create(GRect(0, 80, bounds.size.w - 10, 30));
    next_dose_layer = text_layer_create(GRect(0, 104, bounds.size.w - 10, 30));

    text_layer_set_background_color(next_dose_label_layer, GColorBlack);
    text_layer_set_background_color(next_dose_layer, GColorBlack);
    text_layer_set_text_color(next_dose_label_layer, GColorWhite);
    text_layer_set_text_color(next_dose_layer, GColorWhite);
    text_layer_set_font(next_dose_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_font(next_dose_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(next_dose_label_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(next_dose_layer, GTextAlignmentLeft);

    text_layer_set_text(next_dose_label_layer, "Next Dose in:");

    layer_add_child(window_layer, text_layer_get_layer(next_dose_label_layer));
    layer_add_child(window_layer, text_layer_get_layer(next_dose_layer));
}

static void setup_title_layer(Layer *window_layer, GRect bounds)
{
    title_layer = text_layer_create(GRect(0, STATUS_BAR_LAYER_HEIGHT, bounds.size.w - ACTION_BAR_WIDTH, 60));

    text_layer_set_background_color(title_layer, GColorBlack);
    text_layer_set_text_color(title_layer, GColorWhite);
    text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);

    text_layer_set_text(title_layer, "Dose Reminder");

    layer_add_child(window_layer, text_layer_get_layer(title_layer));
}

static void setup_status_bar(Layer *window_layer, GRect bounds)
{
    status_bar = status_bar_layer_create();

    status_bar_layer_set_colors(status_bar, GColorBlack, GColorWhite);
    status_bar_layer_set_separator_mode(status_bar, StatusBarLayerSeparatorModeDotted);

    layer_add_child(window_layer, status_bar_layer_get_layer(status_bar));
}

static void load_main_window(Window *window)
{
    window_set_background_color(window, GColorBlack);
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    setup_title_layer(window_layer, bounds);
    setup_next_dose_layer(window_layer, bounds);
    setup_main_window_action_bar_layer(window_layer, bounds);
    setup_status_bar(window_layer, bounds);

    set_main_window_layers(next_dose_layer);
}

static void unload_main_window(Window *window)
{
    status_bar_layer_destroy(status_bar);
    text_layer_destroy(title_layer);
    text_layer_destroy(next_dose_layer);
    text_layer_destroy(next_dose_label_layer);
    action_bar_layer_remove_from_window(main_window_action_bar_layer);
    action_bar_layer_destroy(main_window_action_bar_layer);
}

void setup_main_window()
{
    main_window = window_create();

    window_set_window_handlers(main_window, (WindowHandlers) {
        .load = load_main_window,
        .unload = unload_main_window,
        .appear = update_main_window,
        .disappear = disable_main_window
    });

    window_stack_push(main_window, true);
}

void tear_down_main_window()
{
    window_destroy(main_window);
}