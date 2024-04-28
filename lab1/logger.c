#include "logger.h"
#include <time.h>
#include <stdarg.h>
#include <pthread.h>
#include <stdlib.h>


logger_t logger;


void init_logger(log_level_t level, const char *filename, int append, int console) {
    logger.level = level;
    logger.append = append;
    logger.console = console;
    if (filename == NULL) {
        logger.file = stderr;
    } else {
        logger.file = fopen(filename, append ? "a" : "w");
        if (logger.file == NULL) {
            fprintf(stderr, "Не удалось открыть файл %s для логирования\n", filename);
        }
    }
    sem_init(&logger.sem, 0, 1);
}


typedef struct {
    log_level_t level;
    char *func_name;
    char *file_name;
    int line_str;
    char *buffer;
} log_data_t;


void *write_to_file(void *arg) {
    log_data_t *data = (log_data_t *)arg;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char time_str[20];
    strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", tm);
    char *level_str = data->level == ERROR ? "ERROR" : data->level == WARN ? "WARN" : data->level == INFO ? "INFO" : "DEBUG";
    sem_wait(&logger.sem);
    fprintf(logger.file, "%s [%s] %s (%s:%d) %s\n", time_str, level_str, data->func_name, data->file_name, data->line_str, data->buffer);
    if (logger.console) {
        printf("%s [%s] %s (%s:%d) %s\n", time_str, level_str, data->func_name, data->file_name, data->line_str, data->buffer);
    }
    fflush(logger.file);
    sem_post(&logger.sem);
    free(data->func_name);
    free(data->file_name);
    free(data->buffer);
    free(data);
    return NULL;
}


void log_msg(log_level_t level, const char *func, const char *file, int line, const char *msg, ...) {
    if (level <= logger.level) {
        va_list args;
        va_start(args, msg);
        char buffer[256];
        int n = vsnprintf(buffer, 256, msg, args);
        va_end(args);
        if (n > 0) {
            log_data_t *data = malloc(sizeof(log_data_t));
            data->level = level;
            data->func_name = strdup(func);
            data->file_name = strdup(file);
            data->line_str = line;
            data->buffer = strdup(buffer);
            pthread_t thread;
            pthread_create(&thread, NULL, write_to_file, data);
            pthread_detach(thread);
        }
    }
}
