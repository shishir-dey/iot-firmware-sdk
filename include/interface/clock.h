#ifndef IOT_CLOCK_H
#define IOT_CLOCK_H

#include <stdint.h>

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

/**
 * @brief Get current time in microseconds
 *
 * @return uint64_t Current time in microseconds
 */
static inline uint64_t iot_get_time_us(void)
{
    return iot_get_time(IOT_TIME_MICROSECONDS);
}

/**
 * @brief Get current time in milliseconds
 *
 * @return uint64_t Current time in milliseconds
 */
static inline uint64_t iot_get_time_ms(void)
{
    return iot_get_time(IOT_TIME_MILLISECONDS);
}

/**
 * @brief Get current timestamp in seconds
 *
 * @return uint64_t Current timestamp in seconds since epoch
 */
static inline uint64_t iot_get_timestamp(void)
{
    return iot_get_time(IOT_TIME_SECONDS);
}

#endif // IOT_CLOCK_H
