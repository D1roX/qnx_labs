#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include "../include/logger.h"

#define SEC_NSEC 1000000000LL

void *long_thread(void *notused) {
    int time = 20;
    log_info("Поток будет выполняется %d секнуд", time);
    sleep(time);
}

int main(void)
{
    init_logger("lab5.txt", 0, 1);

    uint64_t timeout;
    struct sigevent event;
    int rval;
    pthread_t thread_id;

    log_info("Prog timer");

    event.sigev_notify = SIGEV_UNBLOCK; 
    pthread_create(&thread_id, NULL, long_thread, NULL);

    timeout = 10LL * SEC_NSEC;
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    /*
    Эта функция устанавливает таймер, который сработает через заданное время (timeout).
    CLOCK_REALTIME: указывает, что используется системное время.
    _NTO_TIMEOUT_JOIN: указывает, что таймер будет использован для ожидания завершения потока.
    &event: указатель на структуру sigevent, которая описывает событие, 
    которое произойдет по истечении таймера. В данном случае событие - это разблокировка потока.
    &timeout: указатель на переменную, хранящую время ожидания в наносекундах.
    NULL: указатель на необязательные аргументы, которые не используются в данном случае.
    */
    rval = pthread_join(thread_id, NULL);
    if (rval == ETIMEDOUT) log_info("Поток %d выполняется более 10 секунд", thread_id);
    TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_JOIN, &event, &timeout, NULL);
    sleep(5);
    rval = pthread_join(thread_id, NULL);
    if (rval == ETIMEDOUT) log_info("Поток %d выполняется более 25 секунд", thread_id);
    else log_info("Поток %d завершил работу", thread_id);

    destroy_logger();

    return 1;
}
