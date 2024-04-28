#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>

typedef enum {
    LOG_INFO,
    LOG_DEBUG,
    LOG_WARN,
    LOG_ERROR
} log_level_t;

#define log_info(msg, ...)   log_message(LOG_INFO, __func__, __LINE__, msg, ##__VA_ARGS__)
#define log_debug(msg, ...)  log_message(LOG_DEBUG, __func__, __LINE__, msg, ##__VA_ARGS__)
#define log_warn(msg, ...)   log_message(LOG_WARN, __func__, __LINE__, msg, ##__VA_ARGS__)
#define log_error(msg, ...)  log_message(LOG_ERROR, __func__, __LINE__, msg, ##__VA_ARGS__)

void init_logger(const char *filename, int append, int console);
void destroy_logger();

void log_message(log_level_t level, const char *func, int line, const char *msg, ...);

#endif