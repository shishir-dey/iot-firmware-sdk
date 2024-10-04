#include "clock.h"
#include <time.h>
#include <windows.h>

bb_timestamp_t port_clock_get_timestamp()
{
    return time(NULL);
}

bb_timestamp_us_t port_clock_get_timestamp_us()
{
    return time(NULL) * 1000000;
}

bb_timestamp_ms_t port_clock_get_timestamp_ms()
{
    return time(NULL) * 1000;
}
