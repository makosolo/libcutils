#ifndef H_UTILS_QUEUE_H
#define H_UTILS_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \file
 * \brief Interface to Queue APIs
 */

/*!
 * \brief Flag to indicate queue should block 'put' operation
 *        until free space is available for 'put' to succeed
 *
 */
#define UTIL_QUEUE_FLAG_BLOCK_ON_PUT        (0x00000001u)

/*!
 * \brief Flag to indicate queue should block 'get' operation
 *        until new element is available in queue to extract
 *
 */
#define UTIL_QUEUE_FLAG_BLOCK_ON_GET        (0x00000002u)

/*!
 * \brief Constant to indicate tivxEventPend() should only
 *        check and not wait for event to arrive
 *
 */
#define UTIL_QUEUE_TIMEOUT_NO_WAIT          (0u)

/*!
 * \brief Constant to indicate tivxEventPend() should only
 *        wait forever for the event to arrive
 *
 */
#define UTIL_QUEUE_TIMEOUT_WAIT_FOREVER     (0xFFFFFFFFu)

/*!
 * \brief Typedef for a queue
 *
 */
typedef struct util_queue_s util_queue_t;

int util_queue_create(util_queue_t **queue, uint32_t max_elements, uint32_t flags);
int util_queue_delete(util_queue_t **queue);
int util_queue_put(util_queue_t *queue, uintptr_t data, uint32_t timeout);
int util_queue_pop(util_queue_t *queue, uintptr_t *data, uint32_t timeout);
int util_queue_peek(const util_queue_t *queue, uintptr_t *data);
bool util_queue_isempty(const util_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_QUEUE_H
