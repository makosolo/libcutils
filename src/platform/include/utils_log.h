#ifndef H_UTILS_LOG_H
#define H_UTILS_LOG_H

#include <stdint.h>
#include <stdbool.h>

// Max file size to rotate, should not be more than 4 GB.
#define UTIL_LOG_FILE_SIZE_MAX      (2*1024*1024)
#define UTIL_LOG_FILE_SIZE_MIN      (1*1024)
#define UTIL_LOG_FILE_COUNT_MIN     (2)

typedef enum {
    UTIL_LOG_DEBUG,
    UTIL_LOG_INFO,
    UTIL_LOG_WARN,
    UTIL_LOG_ERROR,
    UTIL_LOG_OFF,
    UTIL_LOG_MAX,
} util_log_level_e;

typedef int (*OnUtilLogCallback)(void *arg, util_log_level_e level, const char *fmt, va_list va);

typedef struct {
    char                tags[64];
	char                file_path[128];
	size_t              file_size;
    uint32_t            file_count;
    bool                stdout_enabled;
    OnUtilLogCallback   onLogCallback;
    void*               arg;
} util_log_cfg_t;

int util_log_init(util_log_cfg_t *cfg);
int util_log_deinit(void);
int util_log_set_level(util_log_level_e level);
int util_log_log(util_log_level_e level, const char *fmt, ...);

// Define SC_LOG_PRINT_FILE_NAME to print file name and line no in the log line.
#ifdef UTIL_LOG_PRINT_FILE_NAME
#define util_log_ap(fmt, ...)                                                    \
	"(%s:%d) " fmt, strrchr("/" __FILE__, '/') + 1, __LINE__, __VA_ARGS__
#else
#define util_log_ap(fmt, ...) fmt, __VA_ARGS__
#endif

#define util_log_debug(...) (util_log_log(UTIL_LOG_DEBUG, util_log_ap(__VA_ARGS__, "")))
#define util_log_info(...) (util_log_log(UTIL_LOG_INFO, util_log_ap(__VA_ARGS__, "")))
#define util_log_warn(...) (util_log_log(UTIL_LOG_WARN, util_log_ap(__VA_ARGS__, "")))
#define util_log_error(...) (util_log_log(UTIL_LOG_ERROR, util_log_ap(__VA_ARGS__, "")))

#define UTIL_ASSERT_GOTO(condition, tag, format, ...)                                                                  \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            fprintf(stderr, "[ERROR][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                          \
            goto tag;                                                                                                  \
        }                                                                                                              \
    } while (0)

#define UTIL_ASSERT_RET(condition, format, ...)                                                                        \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            fprintf(stderr, "[ERROR][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                          \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0)

#define UTIL_ASSERT_RET_VAL(condition, ret, format, ...)                                                               \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            fprintf(stderr, "[ERROR][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                          \
            return ret;                                                                                                \
        }                                                                                                              \
    } while (0)

#define UTIL_ASSERT_CONTINUE(condition, format, ...)                                                                   \
    if (!(condition)) {                                                                                                \
        fprintf(stderr, "[ERROR][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                              \
        continue;                                                                                                      \
    }

#endif // H_UTILS_LOG_H
