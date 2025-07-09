#include <Logger/Logger.h>

#include <uci/uci.h>
#include "config.h"
#include "defines.h"


Settings settings;


uint32_t Settings::get_MonPeriod()
{
    return monPeriod;
}

uint32_t Settings::get_RetryPeriod()
{
    return retryPeriod;
}

double Settings::get_DeltaDistance()
{
    return deltaDistance;
}

uint32_t Settings::get_DeltaBearing()
{
    return deltaBearing;
}

int make_default_config(Settings & psettings)
{
    psettings.set_monPeriod(MON_PERIOD);
    psettings.set_retryPeriod(RETRY_PERIOD);
    psettings.set_deltaDistance(DELTA_DISTANCE);
    psettings.set_deltaBearing(DELTA_BEARING);

    return 0;
}

int load_config(Settings & psettings)
{
    int ret = 0;

    struct uci_context *uci = uci_alloc_context();
    struct uci_package *package = NULL;
    const char *str;
    int cfg_res = -1;

    int config_load_count = 0;
    int config_load_result = -1;

    while (config_load_count < CONFIG_LOAD_ATTEMPTS)
    {
        config_load_count++;
        if (uci_load(uci, SERVICE_CONFIG, &package))
        {
            LOGGER_ERROR("config_load", "config_load: Fail open config (try {:d})!", config_load_count);
            if (config_load_count < CONFIG_LOAD_ATTEMPTS)
            {
                sleep(CONFIG_LOAD_ATTEMPT_TIMEOUT); //wait for next try
            }
            else
            {
                break;
            }
        }
        else
        {
            config_load_result = 0;
            break;
        }
    }

    if (config_load_result < 0)
    {
        LOGGER_INFO("config_load", "Service config open failed");
        return -1;
    }

    LOGGER_INFO("config_load", "Service config open: OK");
    struct uci_element *e;

    uci_foreach_element(&package->sections, e)
    {
        struct uci_section *s = uci_to_section(e);
        if (!strcmp(s->type, SERVICE_NAME_CONFIG))
        {
            int config_errors = 0;
            const char * mon_period = uci_lookup_option_string(uci, s, "MON_PERIOD");
            if (mon_period)
            {
                psettings.set_monPeriod(atoi(mon_period));
                LOGGER_INFO("config_load", "config_load: MON_PERIOD = {:d}", psettings.get_MonPeriod());
            }
            else
            {
                LOGGER_ERROR("config_load", "config_load: MON_PERIOD setting error!");
                config_errors++;
                psettings.set_monPeriod(MON_PERIOD);
            }

            const char * retry_period = uci_lookup_option_string(uci, s, "RETRY_PERIOD");
            if (retry_period)
            {
                psettings.set_retryPeriod(atoi(retry_period));
                LOGGER_INFO("config_load", "config_load: RETRY_PERIOD = {:d}", psettings.get_RetryPeriod());
            }
            else
            {
                LOGGER_ERROR("config_load", "config_load: RETRY_PERIOD setting error!");
                config_errors++;
                psettings.set_retryPeriod(RETRY_PERIOD);
            }

            const char * delta_distance = uci_lookup_option_string(uci, s, "DELTA_DISTANCE");
            if (delta_distance)
            {
                psettings.set_deltaDistance(atof(delta_distance));
                LOGGER_INFO("config_load", "config_load: DELTA_DISTANCE = {:.2f}", psettings.get_DeltaDistance());
            }
            else
            {
                LOGGER_ERROR("config_load", "config_load: DELTA_DISTANCE setting error!");
                config_errors++;
                psettings.set_deltaDistance(DELTA_DISTANCE);
            }

            const char * delta_bearing = uci_lookup_option_string(uci, s, "DELTA_BEARING");
            if (delta_bearing)
            {
                psettings.set_deltaBearing(atoi(delta_bearing));
                LOGGER_INFO("config_load", "config_load: DELTA_BEARING = {:d}", psettings.get_DeltaBearing());
            }
            else
            {
                LOGGER_ERROR("config_load", "config_load: DELTA_BEARING setting error!");
                config_errors++;
                psettings.set_deltaBearing(DELTA_BEARING);
            }

            if (config_errors)
            {
                LOGGER_ERROR("config_load", "config_load: Settings errors = {:d}", config_errors);
                
            }
            else
            {
                LOGGER_INFO("config_load", "config_load: All settings OK");
            }  
        }
    }

    uci_unload(uci, package);
    uci_free_context(uci);
  
    ret = cfg_res = 0;
    return ret;
}

int apply_config(const Settings& psettings)
{
    settings = psettings;
    return 0;
}

uint32_t getMonPeriod()
{
    return settings.get_MonPeriod();
}

uint32_t getRetryPeriod()
{
    return settings.get_RetryPeriod();
}

uint32_t getDeltaDistace()
{
    return settings.get_DeltaDistance();
}

uint32_t getDeltaBearing()
{
    return settings.get_DeltaBearing();
}