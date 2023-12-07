#ifndef H_UTILS_MUTEX_H
#define H_UTILS_MUTEX_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \file
 * \brief Interface to Mutex APIs
 */

/*!
 * \brief Typedef for a mutex
 *
 */
struct util_mutex_s {
  pthread_mutex_t lock;
};

typedef struct util_mutex_s util_mutex_t;

/*!
 * \brief Create a mutex
 *
 * \param mutex [out] Pointer to mutex object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_mutex_create(util_mutex_t *mutex);

/*!
 * \brief Delete a mutex
 *
 * \param mutex [in] Pointer to mutex object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_mutex_destroy(util_mutex_t *mutex);

/*!
 * \brief Lock a mutex
 *
 * \param mutex [in] mutex object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_mutex_lock(util_mutex_t *mutex);

/*!
 * \brief UnLock a mutex
 *
 * \param mutex [in] mutex object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_mutex_unlock(util_mutex_t *mutex);


#ifdef __cplusplus
}
#endif

#endif // H_UTILS_MUTEX_H
