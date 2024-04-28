// logger.h - файл заголовка для логгера

#ifndef LOGGER_H // Проверяем, что файл не включен дважды
#define LOGGER_H

#include <stdio.h>
#include <semaphore.h>

// Определение уровней логирования
typedef enum {
    ERROR,
    WARN,
    INFO,
    DEBUG
} log_level_t;

typedef struct {
    log_level_t level;
    FILE *file;
    int append;
    int console;
    sem_t sem;
} logger_t;

extern logger_t logger;

void init_logger(log_level_t level, const char *filename, int append, int console);

void log_msg(log_level_t level, const char *func, const char *file, int line, const char *msg, ...);

// Макросы, которые вызывают функцию логирования с нужными параметрами
#define log_error(msg, ...) log_msg(ERROR, __func__, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_warn(msg, ...)  log_msg(WARN, __func__, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_info(msg, ...)  log_msg(INFO, __func__, __FILE__, __LINE__, msg, ##__VA_ARGS__)
#define log_debug(msg, ...) log_msg(DEBUG, __func__, __FILE__, __LINE__, msg, ##__VA_ARGS__)

#endif // LOGGER_H
