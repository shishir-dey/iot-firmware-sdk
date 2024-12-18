#ifndef IOT_OS_H
#define IOT_OS_H

#include <stdint.h>

// Opaque thread type
struct iot_thread;
typedef struct iot_thread iot_thread;

// Thread function type
typedef void (*iot_thread_func_t)(void* arg);

/**
 * @brief Create a new thread
 *
 * @param name Name of the thread
 * @param func Thread function to execute
 * @param arg Argument passed to thread function
 * @param priority Thread priority
 * @param stack_size Stack size in bytes
 * @param event_count Maximum number of events thread can handle
 * @return struct iot_thread* Thread handle on success, NULL on failure
 */
struct iot_thread* iot_thread_create(const char* name, iot_thread_func_t func, void* arg,
    uint32_t priority, uint32_t stack_size, uint32_t event_count);

/**
 * @brief Delete a thread
 *
 * @param thread Thread handle to delete
 * @return int 0 on success, negative value on error
 */
int iot_thread_delete(struct iot_thread* thread);

/**
 * @brief Set thread priority
 *
 * @param thread Thread handle
 * @param priority New priority value
 * @return int 0 on success, negative value on error
 */
int iot_thread_set_priority(struct iot_thread* thread, uint32_t priority);

/**
 * @brief Get thread priority
 *
 * @param thread Thread handle
 * @param priority Pointer to store priority value
 * @return int 0 on success, negative value on error
 */
int iot_thread_get_priority(struct iot_thread* thread, uint32_t* priority);

/**
 * @brief Suspend thread execution
 *
 * @param thread Thread handle
 * @return int 0 on success, negative value on error
 */
int iot_thread_suspend(struct iot_thread* thread);

/**
 * @brief Resume thread execution
 *
 * @param thread Thread handle
 * @return int 0 on success, negative value on error
 */
int iot_thread_resume(struct iot_thread* thread);

/**
 * @brief Delay thread execution
 *
 * @param milliseconds Time to delay in milliseconds
 */
void iot_thread_delay(uint32_t milliseconds);

/**
 * @brief Wait for thread completion
 *
 * @param thread Thread handle
 * @param retval Pointer to store return value
 * @return int 0 on success, negative value on error
 */
int iot_thread_join(struct iot_thread* thread, void** retval);

// Opaque mutex type
struct iot_mutex;

/**
 * @brief Initialize mutex
 *
 * @return struct iot_mutex* Mutex handle on success, NULL on failure
 */
struct iot_mutex* iot_mutex_init(void);

/**
 * @brief Lock mutex
 *
 * @param mutex Mutex handle
 * @return int 0 on success, negative value on error
 */
int iot_mutex_lock(struct iot_mutex* mutex);

/**
 * @brief Unlock mutex
 *
 * @param mutex Mutex handle
 * @return int 0 on success, negative value on error
 */
int iot_mutex_unlock(struct iot_mutex* mutex);

/**
 * @brief Destroy mutex
 *
 * @param mutex Mutex handle
 */
void iot_mutex_destroy(struct iot_mutex* mutex);

#endif // IOT_OS_H