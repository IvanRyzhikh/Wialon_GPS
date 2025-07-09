#include <chrono>

/*
 Time api
*/
uint64_t get_current_time_ms(void)
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

uint32_t get_timestamp_s(void)
{
    using namespace std::chrono;
    return (uint32_t)((duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch()).count())/1000);
}