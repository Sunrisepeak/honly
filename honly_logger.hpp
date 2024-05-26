#ifndef LOGGER_HPP__HONLY
#define LOGGER_HPP__HONLY

#include <stdio.h>

#ifndef HONLY_LOGGER_TAG
#define HONLY_LOGGER_TAG "HONLY"
#endif

#define LOG_ENABLE true

#define HONLY_LOG_(fd, ...) \
do { \
    if (LOG_ENABLE) { \
        fprintf (fd, __VA_ARGS__); \
        fprintf (fd, "\033[0m\n"); \
    } \
} while(0)

#define HONLY_LOGI(...) \
do { \
    fprintf(stdout, "\033[32m[%s LOGI]: %s: %s:%d - ", \
        HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); \
    HONLY_LOG_(stdout, __VA_ARGS__); \
} while (0)

#define HONLY_LOGD(...) \
do { \
    fprintf(stdout, "\033[37m[%s LOGD]: %s: %s:%d - ", \
        HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); \
    HONLY_LOG_(stdout, __VA_ARGS__); \
} while (0)

#define HONLY_LOGW(...) \
do { \
    fprintf(stdout, "\033[33m[%s LOGW]: %s: %s:%d - ", \
        HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); \
    HONLY_LOG_(stdout, __VA_ARGS__); \
} while (0)

#define HONLY_LOGE(...) \
do { \
    fprintf(stderr, "\033[31m[%s LOGE]: %s: %s:%d - ", \
        HONLY_LOGGER_TAG, __func__, __FILE__, __LINE__); \
    HONLY_LOG_(stdout, __VA_ARGS__); \
} while (0)

#endif