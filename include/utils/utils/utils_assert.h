#ifndef H_UTILS_ASSERT_H
#define H_UTILS_ASSERT_H

#include <stdio.h>

#define UTIL_ASSERT_GOTO(condition, tag, format, ...)                                                                  \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            fprintf(stderr, "[ASSERT][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                          \
            goto tag;                                                                                                  \
        }                                                                                                              \
    } while (0)

#define UTIL_ASSERT_RET(condition, format, ...)                                                                        \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            fprintf(stderr, "[ASSERT][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                          \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0)

#define UTIL_ASSERT_RET_VAL(condition, ret, format, ...)                                                               \
    do {                                                                                                               \
        if (!(condition)) {                                                                                            \
            fprintf(stderr, "[ASSERT][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                          \
            return ret;                                                                                                \
        }                                                                                                              \
    } while (0)

#define UTIL_ASSERT_CONTINUE(condition, format, ...)                                                                   \
    if (!(condition)) {                                                                                                \
        fprintf(stderr, "[ASSERT][%s:%d]" format "\n", __func__, __LINE__, ##__VA_ARGS__);                              \
        continue;                                                                                                      \
    }

#endif // H_UTILS_ASSERT_H
