#ifndef H_UTILS_EVENT_H
#define H_UTILS_EVENT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Constant to indicate tivxEventPend() should only
 *        check and not wait for event to arrive
 *
 */
#define UTIL_EVENT_TIMEOUT_NO_WAIT          (0u)

/*!
 * \brief Constant to indicate tivxEventPend() should only
 *        wait forever for the event to arrive
 *
 */
#define UTIL_EVENT_TIMEOUT_WAIT_FOREVER     (0xFFFFFFFFu)

/*!
 * \brief Typedef for a event
 *
 */
struct util_event_s;
typedef struct util_event_s util_event_t;

/*!
 * \brief Create a event
 *
 * \param event [out] Pointer to event object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_event_create(util_event_t **event);

/*!
 * \brief Delete a event
 *
 * \param event [in] Pointer to event object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_event_delete(util_event_t **event);

/*!
 * \brief Post a event
 *
 * \param event [in] event object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_event_post(util_event_t *event);

/*!
 * \brief Pend on a event
 *
 * \param event [in] event object
 * \param timeout [in] Timeout in units of msecs,
 *                     use TIVX_EVENT_TIMEOUT_WAIT_FOREVER to wait forever
 *
 * \return VX_SUCCESS on success
 *
 */
int util_event_wait(util_event_t *event, uint32_t timeout);

/*!
 * \brief Clear any pending events
 *
 * \param event [in] event object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_event_clear(util_event_t *event);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_EVENT_H
