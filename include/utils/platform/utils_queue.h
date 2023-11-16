#ifndef H_UTILS_QUEUE_H
#define H_UTILS_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#include "utils_event.h"
#include "utils_mutex.h"

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
struct util_queue_s {

  uint32_t cur_rd;
  /**< Current read index */

  uint32_t cur_wr;
  /**< Current write index  */

  uint32_t count;
  /**< Count of element in queue  */

  uint32_t max_ele;
  /**< Max elements that be present in the queue  */

  uintptr_t *queue;
  /**< Address of data area of the queue elements */

  util_event_t* block_rd;
  /**< Read semaphore */

  util_event_t* block_wr;
  /**< Write semaphore  */

  util_mutex_t* lock;
  /**< Queue lock semaphore  */

  void *context;
  /**< Private context of queue handle */

  uint32_t flags;
  /**< Controls how APIs behave internally, i.e blocking wait or non-blocking */

  volatile bool blockedOnGet;
  /**< Flag indicating queue is blocked on get operation */

  volatile bool blockedOnPut;
  /**< Flag indicating queue is blocked on put operation */
};

typedef struct util_queue_s util_queue_t;

int util_queue_create(util_queue_t *queue, uint32_t max_elements, uintptr_t *queue_memory, uint32_t flags);
int util_queue_destroy(util_queue_t *queue);
int util_queue_put(util_queue_t *queue, uintptr_t data, uint32_t timeout);
int util_queue_pop(util_queue_t *queue, uintptr_t *data, uint32_t timeout);
int util_queue_peek(const util_queue_t *queue, uintptr_t *data);
bool util_queue_isempty(const util_queue_t *queue);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_QUEUE_H
