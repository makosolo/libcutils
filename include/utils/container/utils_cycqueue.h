#ifndef H_UTILS_CYCQUEUE_H
#define H_UTILS_CYCQUEUE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Typedef for a queue
 *
 */
struct util_cycqueue_s {
  uint32_t  cur_rd;
  uint32_t  cur_wr;
  uint32_t  count;
  uint32_t  max_ele;
  uintptr_t *queue;
};

typedef struct util_cycqueue_s util_cycqueue_t;

int util_cycqueue_create(util_cycqueue_t *queue, uint32_t max_elements);
int util_cycqueue_destroy(util_cycqueue_t *queue);
int util_cycqueue_put(util_cycqueue_t *queue, uintptr_t data);
int util_cycqueue_pop(util_cycqueue_t *queue, uintptr_t *data);
int util_cycqueue_peek(const util_cycqueue_t *queue, uintptr_t *data);
bool util_cycqueue_isempty(const util_cycqueue_t *queue);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_CYCQUEUE_H
