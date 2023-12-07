#ifndef H_UTILS_TASK_POOL_H
#define H_UTILS_TASK_POOL_H

#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Entry point of task
 *
 *  \param target [in] target handle
 *
 */
typedef void (*OnUtilTaskPollCallback)(void *app_var);

/* Binary semaphore */
typedef struct util_task_pool_bsem_s {
	pthread_mutex_t           mutex;
	pthread_cond_t            cond;
	int                       v;
} util_task_pool_bsem_t;

/* Job */
typedef struct util_task_pool_job_s {
	struct util_task_pool_job_s* prev;  /* pointer to previous job   */
	OnUtilTaskPollCallback    callback; /* function pointer          */
	void*                     arg;      /* function's argument       */
} util_task_pool_job_t;

/* Job queue */
typedef struct util_task_pool_jobqueue_s {
	pthread_mutex_t           rwmutex;  /* used for queue r/w access */
	util_task_pool_job_t*     front;    /* pointer to front of queue */
	util_task_pool_job_t*     rear;     /* pointer to rear  of queue */
	util_task_pool_bsem_t*    has_jobs; /* flag as binary semaphore  */
	int                       len;      /* number of jobs in queue   */
} util_task_pool_jobqueue_t;

/* Thread */
typedef struct util_task_pool_thread_s {
	int                       id;       /* friendly id               */
	pthread_t                 pthread;  /* pointer to actual thread  */
	struct util_task_pool_s*  thpool_p; /* access to thpool          */
} util_task_pool_thread_t;

/* Threadpool */
typedef struct util_task_pool_s {
	util_task_pool_thread_t** threads;             /* pointer to threads        */
	volatile int              num_threads_alive;   /* threads currently alive   */
	volatile int              num_threads_working; /* threads currently working */
	pthread_mutex_t           thcount_lock;        /* used for thread count etc */
	pthread_cond_t            threads_all_idle;    /* signal to thpool_wait     */
	util_task_pool_jobqueue_t jobqueue;            /* job queue                 */
} util_task_pool_t;

/*!
 * \brief Create a task
 *
 * \param tpoll [out] Pointer to task pool object
 * \param num_threads [in] number of threads to be created in the threadpool
 *
 * \return VX_SUCCESS on success
 *
 */
int util_task_pool_create(util_task_pool_t *tpoll, int num_threads);

/*!
 * \brief Delete a task pool
 *
 * \param task [in] Pointer to task object
 *
 * \return VX_SUCCESS on success
 *
 */
int util_task_pool_destroy(util_task_pool_t *tpoll);

/**
 * @brief Add work to the job queue
 *
 * Takes an action and its argument and adds it to the threadpool's job queue.
 * If you want to add to work a function with more than one arguments then
 * a way to implement this is by passing a pointer to a structure.
 * 
 * @param  tpoll         threadpool to which the work will be added
 * @param  function_p    pointer to function to add as work
 * @param  arg           pointer to an argument
 * @return 0 on success, -1 otherwise.
 */
int util_task_pool_add(util_task_pool_t *tpoll, OnUtilTaskPollCallback callback, void* arg);

/**
 * @brief Wait for all queued jobs to finish
 *
 * Will wait for all jobs - both queued and currently running to finish.
 * Once the queue is empty and all work has completed, the calling thread
 * (probably the main program) will continue.
 *
 * Smart polling is used in wait. The polling is initially 0 - meaning that
 * there is virtually no polling at all. If after 1 seconds the threads
 * haven't finished, the polling interval starts growing exponentially
 * until it reaches max_secs seconds. Then it jumps down to a maximum polling
 * interval assuming that heavy processing is being used in the threadpool.
 *
 * @param tpoll         the threadpool to wait for
 * @return nothing
 */
int util_task_pool_wait(util_task_pool_t *tpoll);

/**
 * @brief Pauses all threads immediately
 *
 * The threads will be paused no matter if they are idle or working.
 * The threads return to their previous states once thpool_resume
 * is called.
 *
 * While the thread is being paused, new work can be added.
 *
 * @param tpoll          the threadpool where the threads should be paused
 * @return nothing
 */
int util_task_pool_pause(util_task_pool_t *tpoll);

/**
 * @brief Unpauses all threads if they are paused
 *
 * @param tpoll          the threadpool where the threads should be unpaused
 * @return nothing
 */
int util_task_pool_resume(util_task_pool_t *tpoll);

/**
 * @brief Show currently working threads
 *
 * Working threads are the threads that are performing work (not idle).
 *
 * @param tpoll          the threadpool of interest
 * @return integer       number of threads working
 */
int util_task_pool_num_working(util_task_pool_t *tpoll);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_TASK_POOL_H
