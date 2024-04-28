#include <stdio.h>
#include <time.h>
#include <sync.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include "../include/logger.h"

barrier_t barrier;

void *thread1(void * not_used)
{
    char buf[27];
    time_t now;

    log_info("Начало ожидания барьера в 1 потоке");
    sleep(1);
    barrier_wait(&barrier);
    log_info("Окончание ожидания барьера в 1 потоке");
}

void *thread2(void * not_used)
{
    char buf[27];
    time_t now;

    log_info("Начало ожидания барьера во 2 потоке");
    sleep(2);
    barrier_wait(&barrier);
    log_info("Окончание ожидания барьера во 2 потоке");
}


int main()
{
    init_logger("lab4.txt", 0, 1);

    char buf[27];
    time_t now;
    int NUM_THREADS = 2;
    pthread_t t1;
    pthread_t t2;

    //барьер, атрибуты, число потоков + 1, т.к. мейн поток
    barrier_init(&barrier, NULL, NUM_THREADS + 1); 
    log_info("Start programm");
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    log_info("Начало ожидания барьера в main");
    barrier_wait(&barrier);
    log_info("Окончание ожидания барьера в main");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    destroy_logger();

    return 0;
}