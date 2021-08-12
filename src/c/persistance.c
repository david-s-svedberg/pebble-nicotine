#include "persistance.h"

#include <pebble.h>
#include <stdbool.h>

#include "format.h"

static const uint32_t DATA_KEY = 738742;
static Data m_data;
static bool m_data_loaded = false;

static void seed_data()
{
    m_data.intervalMinutes = 60;
    m_data.last_hour = 22;
    m_data.last_minute = 0;

    persist_write_data(DATA_KEY, &m_data, sizeof(Data));
}

Data* get_data()
{
    if(!has_any_data())
    {
        seed_data();
    }
    if(!m_data_loaded)
    {
        persist_read_data(DATA_KEY, &m_data, sizeof(Data));
        m_data_loaded = true;
    }

    return &m_data;
}

bool has_next_dose_time()
{
    Data* data = get_data();
    time_t wakup_time;
    return wakeup_query(data->next_wakeup_id, &wakup_time);
}

time_t get_next_wakup_time()
{
    time_t wakup_time;
    Data* data = get_data();
    wakeup_query(data->next_wakeup_id, &wakup_time);
    return wakup_time;
}

uint16_t get_minutes_until_next_dose()
{
    time_t wakup_time;
    Data* data = get_data();
    wakeup_query(data->next_wakeup_id, &wakup_time);
    return (wakup_time - time(NULL)) / SECONDS_PER_MINUTE;
}

bool has_any_data()
{
    return persist_exists(DATA_KEY);
}

void save_data()
{
    persist_write_data(DATA_KEY, &m_data, sizeof(Data));
}

void fill_next_dose_time(char* buf)
{
    if(!has_next_dose_time())
    {
        snprintf(buf, 6, "None");
    } else
    {
        uint16_t minutes_until_next_dose = get_minutes_until_next_dose();
        uint8_t hours = minutes_until_next_dose / MINUTES_PER_HOUR;
        uint8_t minutes = minutes_until_next_dose % MINUTES_PER_HOUR;
        fill_time_string(buf, hours, minutes);
    }
}