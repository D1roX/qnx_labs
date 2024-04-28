#include "../include/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#define MAX_QUEUE_SIZE 1024
#define MAX_LOG_MESSAGE_LENGTH 1024

typedef struct {
    char time[20];
    log_level_t level;
    char func[64];
    int line;
    char message[MAX_LOG_MESSAGE_LENGTH];
} log_message_t;

FILE *log_file;
pthread_t logger_thread;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t log_cond = PTHREAD_COND_INITIALIZER;
int exit_flag = 0;
log_message_t message_queue[MAX_QUEUE_SIZE];
int queue_head = 0;
int queue_tail = 0;

int is_console_enabled = 1;

const char *level_strings[] = {
    "INFO",
    "DEBUG",
    "WARN",
    "ERROR"
};

void *logger_thread_func(void *arg) {
    while (!exit_flag || queue_head != queue_tail) {
        pthread_mutex_lock(&log_mutex);

        while (queue_head == queue_tail && !exit_flag) {
            pthread_cond_wait(&log_cond, &log_mutex);
        }

        if (queue_head != queue_tail) {
            log_message_t message = message_queue[queue_head];
            queue_head = (queue_head + 1) % MAX_QUEUE_SIZE;

            fprintf(log_file, "%s %s (%s:%d) %s\n", message.time, level_strings[message.level], message.func, message.line, message.message);
            if (is_console_enabled) {
                printf("%s %s (%s:%d) %s\n", message.time, level_strings[message.level], message.func, message.line, message.message);
            }
            fflush(log_file);
        }
        pthread_mutex_unlock(&log_mutex);
    }


    return NULL;
}

void init_logger(const char *filename, int append, int console) {
    log_file = fopen(filename, append ? "a" : "w");
    if (!log_file) {
        perror("fopen");
        exit(1);
    }
    is_console_enabled = console;

    pthread_mutex_init(&log_mutex, NULL);
    pthread_create(&logger_thread, NULL, logger_thread_func, NULL);
}

void destroy_logger() {
    exit_flag = 1;
    pthread_cond_broadcast(&log_cond);
    pthread_join(logger_thread, NULL);
    pthread_mutex_destroy(&log_mutex);
    fclose(log_file);
}

void log_message(log_level_t level, const char *func, int line, const char *msg, ...) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    va_list args;
    va_start(args, msg);

    pthread_mutex_lock(&log_mutex);

    log_message_t *message = &message_queue[queue_tail];
    strftime(message->time, 20, "%Y-%m-%d %H:%M:%S", tm);
    message->level = level;
    strncpy(message->func, func, sizeof(message->func) - 1);
    message->line = line;
    vsnprintf(message->message, sizeof(message->message), msg, args);

    queue_tail = (queue_tail + 1) % MAX_QUEUE_SIZE;

    pthread_cond_signal(&log_cond);
    pthread_mutex_unlock(&log_mutex);

    va_end(args);
}