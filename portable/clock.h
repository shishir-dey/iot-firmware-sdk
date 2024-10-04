#include "custom_types.h"

/**
 * @brief Get the current timestamp in seconds
 *
 * @return bb_timestamp_t Current timestamp in seconds
 */
bb_timestamp_t port_clock_get_timestamp();

/**
 * @brief Get the current timestamp in microseconds
 *
 * @return bb_timestamp_us_t Current timestamp in microseconds
 */
bb_timestamp_us_t port_clock_get_timestamp_us();

/**
 * @brief Get the current timestamp in milliseconds
 *
 * @return bb_timestamp_ms_t Current timestamp in milliseconds
 */
bb_timestamp_ms_t port_clock_get_timestamp_ms();
