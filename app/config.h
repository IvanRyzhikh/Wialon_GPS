#pragma once

#include <cstdint>

#define MON_PERIOD 5
#define RETRY_PERIOD 1
#define DELTA_DISTANCE 100.0
#define DELTA_BEARING 30

class Settings
{
private:
    uint32_t monPeriod;
    uint32_t retryPeriod;
    double deltaDistance;
    uint32_t deltaBearing;

public:
    void set_monPeriod(uint32_t v)
    {
        monPeriod = v * 1000;
    }

    void set_retryPeriod(uint32_t v)
    {
        retryPeriod = v * 1000;
    }

    void set_deltaBearing(uint32_t v)
    {
        deltaBearing = v * 100;
    }

    void set_deltaDistance(double v)
    {
        deltaDistance = v * 1;
    }

    uint32_t get_MonPeriod();
    uint32_t get_RetryPeriod();
    double get_DeltaDistance();
    uint32_t get_DeltaBearing();

    
};

int make_default_config(Settings & psettings);
int load_config(Settings & psettings);
int apply_config(const Settings & psettings);


uint32_t getMonPeriod();
uint32_t getRetryPeriod();
uint32_t getDeltaDistace();
uint32_t getDeltaBearing();