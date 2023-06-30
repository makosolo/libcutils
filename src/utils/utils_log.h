#ifndef H_UTILS_MESSAGES_H
#define H_UTILS_MESSAGES_H

#include <stdint.h>
#include <stdbool.h>

// int util_log_init();
// int util_log_deinit();
// int util_log_set_level();
// int util_log_log(enum sc_log_level level, const char *fmt, ...);

// #define util_log_debug(...) (sc_log_log(SC_LOG_DEBUG, sc_log_ap(__VA_ARGS__, "")))
// #define util_log_info(...) (sc_log_log(SC_LOG_INFO, sc_log_ap(__VA_ARGS__, "")))
// #define util_log_warn(...) (sc_log_log(SC_LOG_WARN, sc_log_ap(__VA_ARGS__, "")))
// #define util_log_error(...) (sc_log_log(SC_LOG_ERROR, sc_log_ap(__VA_ARGS__, "")))

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

#endif // H_UTILS_MESSAGES_H
