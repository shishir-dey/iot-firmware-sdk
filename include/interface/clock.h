#ifndef IOT_CLOCK_H
#define IOT_CLOCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Time unit enumeration
 */
enum iot_time_unit {
    IOT_TIME_MICROSECONDS, /**< Microseconds */
    IOT_TIME_MILLISECONDS, /**< Milliseconds */
    IOT_TIME_SECONDS /**< Seconds timestamp */
};

/**
 * @brief Get current time in specified units
 *
 * @param unit Time unit to return
 * @return uint64_t Current time in requested units
 */
uint64_t iot_get_time(enum iot_time_unit unit);

#ifdef __cplusplus
}
#endif

#endif // IOT_CLOCK_H
