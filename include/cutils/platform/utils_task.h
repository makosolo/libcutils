#ifndef H_UTILS_TASK_H
#define H_UTILS_TASK_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Constant to define highest priority for a task
 *
 */
#define UTIL_TASK_PRI_HIGHEST   (0u)

/*!
 * \brief Constant to define lowest priority for a task
 *
 */
#define UTIL_TASK_PRI_LOWEST    (15u)

/*!
 * \brief constant to indicate task affinity can be decided by OS on a SMP CPU
 *
 */
#define UTIL_TASK_AFFINITY_ANY      (0xFFFFu)

/*!
 * \brief Max Task Name Size
 *
 */
#define UTIL_MAX_TASK_NAME         (64u)

/*! \brief Entry point of task
 *
 *  \param target [in] target handle
 *
 */
typedef void (*OnUtilTaskMainCallback)(void *app_var);

/*!
 * \brief Parameters that can be set during task creation
 *
 */
struct util_task_create_params_s {
    /*! \brief Pointer to task stack, if NULL then task stack
     *         is allcoated by OS and not supplied by user
     */
    uint8_t *stack_ptr;

    /*! \brief Task stack size, if 0, OS allocates stack with default size
     */
    uint32_t stack_size;

    /*! \brief If task runs on a SMP CPU then this value tells the affinity
     *         of task to a given core,
     *         Valid values are 0 .. max cores in the SMP CPU.
     *         when TIVX_TASK_AFFINITY_ANY is used OS decides the task affinity.
     */
    uint32_t core_affinity;

    /*! \brief task priority for task associated with this target
     *         TIVX_TASK_PRI_HIGHEST is highest priority,
     *         TIVX_TASK_PRI_LOWEST is lowest priority
     */
    uint32_t priority;

    /*! \brief Entry point for task */
    OnUtilTaskMainCallback task_main;

    /*! \brief private app object */
    void *app_var;


    /*! \brief Task Name */
    char task_name[UTIL_MAX_TASK_NAME];
};

typedef struct util_task_create_params_s util_task_create_params_t;

struct util_task_s {
    /*! \brief Handle to the task created
     */
    void *tsk_handle;

    /*! \brief Pointer to task stack, if NULL then task stack
     *         is allcoated by OS and not supplied by user
     */
    uint8_t *stack_ptr;

    /*! \brief Task stack size, if 0, OS allocates stack with default size
     */
    uint32_t stack_size;

    /*! \brief If task runs on a SMP CPU then this value tells the affinity
     *         of task to a given core,
     *         Valid values are 0 .. max cores in the SMP CPU.
     *         when TIVX_TASK_AFFINITY_ANY is used OS decides the task affinity.
     */
    uint32_t core_affinity;

    /*! \brief task priority for task associated with this target
     *         TIVX_TASK_PRI_HIGHEST is highest priority,
     *         TIVX_TASK_PRI_LOWEST is lowest priority
     */
    uint32_t priority;

    /*! \brief Entry point for task */
    OnUtilTaskMainCallback task_func;

    /*! \brief private app object */
    void *app_var;

    /*! \brief Task name */
    char  task_name[UTIL_MAX_TASK_NAME];
};

typedef struct util_task_s util_task_t;

/*!
 * \brief Create a task
 *
 * \param task [out] Pointer to task object
 * \param params [in] Task create parameters
 *
 * \return VX_SUCCESS on success
 *
 */
int util_task_create(util_task_t *task, const util_task_create_params_t *params);

/*!
 * \brief Delete a task
 *
 * \param task [in] Pointer to task object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_task_destroy(util_task_t *task);

/*!
 * \brief waits/sleeps for given milliseconds
 *
 * \param msec [in] amount of milliseconds to sleep
 *
 */
void util_task_wait_msecs(uint32_t msec);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_TASK_H
