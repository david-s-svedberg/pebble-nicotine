#include "persistance.h"

#include <pebble.h>
#include <stdbool.h>

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