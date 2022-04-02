#include "config_menu_window.h"

#include <pebble.h>

#include "config_menu_window_logic.h"

static Window *config_window;

static StatusBarLayer* status_bar;

static SimpleMenuLayer* alarms_menu_layer;

static SimpleMenuSection m_menu[1];

static SimpleMenuSection m_settings_section;

static SimpleMenuItem m_settings_items[4];

static SimpleMenuItem m_interval_item;
static SimpleMenuItem m_last_alarm_item;
static SimpleMenuItem m_next_alarm_item;
static SimpleMenuItem m_timeout_item;

static void refresh_menu()
{
    layer_mark_dirty(simple_menu_layer_get_layer(alarms_menu_layer));
}

static void update_menu_items()
{
    update_interval_item(&m_interval_item);
    update_next_alarm_item(&m_next_alarm_item);
    update_last_alarm_item(&m_last_alarm_item);
    update_timeout_item(&m_timeout_item);
    refresh_menu();
}

static void update_config_menu(Window* config_window)
{
    update_menu_items();
}

static void handle_timeout_pressed(int index, void* context)
{
    handle_timeout_edit();
    update_menu_items();
}

static void setup_interval_item()
{
    m_interval_item.title = "Intervall";
    m_interval_item.subtitle = get_interval_text();
    m_interval_item.callback = handle_interval_edit;
    m_interval_item.icon = NULL;
}

static void setup_last_alarm_item()
{
    m_last_alarm_item.title = "Last Alarm Time";
    m_last_alarm_item.subtitle = get_last_alarm_text();
    m_last_alarm_item.callback = handle_last_alarm_edit;
    m_last_alarm_item.icon = NULL;
}

static void setup_next_alarm_item()
{
    m_next_alarm_item.title = "Next Alarm Time";
    m_next_alarm_item.subtitle = get_next_alarm_text();
    m_next_alarm_item.callback = handle_next_alarm_edit;
    m_next_alarm_item.icon = NULL;
}

static void setup_timeout_item()
{
    m_timeout_item.title = "Alarm Timeout";
    m_timeout_item.subtitle = get_timeout_text();
    m_timeout_item.callback = handle_timeout_pressed;
    m_timeout_item.icon = NULL;
}

static void setup_alarms_menu_layer(Layer *window_layer, GRect bounds)
{
    setup_interval_item();
    m_settings_items[0] = m_interval_item;

    setup_last_alarm_item();
    m_settings_items[1] = m_last_alarm_item;

    setup_next_alarm_item();
    m_settings_items[2] = m_next_alarm_item;

    setup_timeout_item();
    m_settings_items[3] = m_timeout_item;

    m_settings_section.num_items = 4;
    m_settings_section.title = "Settings";
    m_settings_section.items = m_settings_items;

    m_menu[0] = m_settings_section;
    alarms_menu_layer = simple_menu_layer_create(
        GRect(0, STATUS_BAR_LAYER_HEIGHT, bounds.size.w, bounds.size.h - STATUS_BAR_LAYER_HEIGHT),
        config_window,
        m_menu, 1, NULL);
    layer_add_child(window_layer, simple_menu_layer_get_layer(alarms_menu_layer));
}

static void setup_status_bar(Layer *window_layer, GRect bounds)
{
    status_bar = status_bar_layer_create();

    status_bar_layer_set_colors(status_bar, GColorBlack, GColorWhite);
    status_bar_layer_set_separator_mode(status_bar, StatusBarLayerSeparatorModeDotted);

    layer_add_child(window_layer, status_bar_layer_get_layer(status_bar));
}

static void load_config_menu_window(Window *config_window)
{
    window_set_background_color(config_window, GColorBlack);
    Layer *config_window_layer = window_get_root_layer(config_window);
    GRect config_window_bounds = layer_get_bounds(config_window_layer);

    setup_alarms_menu_layer(config_window_layer, config_window_bounds);
    setup_status_bar(config_window_layer, config_window_bounds);
}

static void unload_config_menu_window(Window *window)
{
    simple_menu_layer_destroy(alarms_menu_layer);
    status_bar_layer_destroy(status_bar);
}

void setup_config_menu_window()
{
    config_window = window_create();

    window_set_window_handlers(config_window, (WindowHandlers) {
        .load = load_config_menu_window,
        .unload = unload_config_menu_window,
        .appear = update_config_menu
    });

    window_stack_push(config_window, true);
}

void tear_down_config_menu_window()
{
    window_destroy(config_window);
}