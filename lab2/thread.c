#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include "../include/logger.h"

typedef struct thread_args {
    int id;
    int n;
    const char* msg;
} thread_args;

void *thread_func(void *args) {    
    thread_args* cur_args = (thread_args*)args;
    log_info("Поток %d: начал выполнение. n = %d", cur_args->id, cur_args->n);
    log_info("Поток %d: %s", cur_args->id, cur_args->msg);
    // for (long long int i = 0, j = 0; i < cur_args->n * 2500000000; i++, j++);
    sleep(cur_args->n);
    log_info("Поток %d: завершил выполнение", cur_args->id);
    pthread_exit(NULL);
}


void create_thread(pthread_t* tid, int policy, int prio, void*(func)(void*), void* args)
{
    pthread_attr_t attr;
    struct sched_param param;

    param.sched_priority = prio;
    pthread_attr_init(&attr);
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setschedpolicy(&attr, policy);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(tid, &attr, func, args);

    pthread_attr_destroy(&attr);
}


int main() {
    init_logger("thread.txt", 0, 1);

    pthread_t thread_id1, thread_id2;
    pthread_attr_t attr1, attr2;
    struct sched_param param1, param2;
    int priority_1 = 10;
    int priority_2 = 20;

    thread_args args1 = {1, 5, "Сообщение из первого потока"};
    thread_args args2 = {2, 10, "Сообщение из второго потока"};

    create_thread(&thread_id1, SCHED_FIFO, priority_1, thread_func, (void*)&args1);
    // sleep(1);
    create_thread(&thread_id2, SCHED_FIFO, priority_2, thread_func, (void*)&args2);

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);
    destroy_logger();
    return 0;
}
