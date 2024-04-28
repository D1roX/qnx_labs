#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "../include/logger.h"


#define MEM_NAME "ourmemory"

int threads(void);
void *thread_1(void *);
void *thread_2(void *);
int process_1(void);
int process_2(void);

typedef struct {
    sem_t sem;
    int value;
} sh_mem_t;

pthread_mutex_t mut;
sh_mem_t *sm;

int main(void) {
    // Инициализация

    int md;
    pid_t childpid;

    // На слуйчай не удаленного блока памяти
    shm_unlink(MEM_NAME);

    // Создание разделяемого блока памяти
    /*
        name - Имя объекта в разделяемой области памяти, который необходимо открыть.
        oflag - Комбинация следующих битов (определены в <fcntl.h>):
                    O_RDWR - открытие для чтения и записи.
                    O_CREAT - если объект разделяемой памяти существует, этот флаг не действует. В противном случае создается объект 
                    в разделяемой области памяти и его права устанавливаются в соответствии со значением mode и маской создания режима файла для процесса.
        mode - Биты разрешения (права) для объекта памяти устанавливаются в значение mode, за исключением тех битов, которые установлены в маске создания файла для процесса.
    */
       
    md = shm_open(MEM_NAME, O_RDWR | O_CREAT, 0777);
    if (md == -1) {
        perror("shm_open");
        return -1;
    }

    // Установка размера
    if (ftruncate(md, sizeof(*sm)) == -1) {
        perror("ftruncate");
        return -1;
    }

    // Разметка объекта памяти
    /*
        addr - NULL или указатель на виртуальный адрес, по которому должна быть смапирована память в вызывающем процессе.
        len - 0 или количество байт данных, подлежащих мапированию.
        prot - Возможности доступа, которые необходимо использовать для мапируемой области памяти. Может быть использована комбинация следующих битов, определённых в <sys/mman.h>
                    PROT_READ - область помечается в качестве доступной для чтения.
                    PROT_WRITE - область помечается в качестве доступной для записи.
        flags - Флаги, определяющие дополнительную информацию для обработки области памяти. POSIX определяет следующие флаги
                    MAP_SHARED - разделяемая память
        fildes - Файловый дескриптор, указывающий на файл, объект разделяемой памяти или объект типизированной памяти. Если мапируется анонимная или физическая память, аргумент должен принимать значение NOFD.
        off - Смещение в файле или объекте памяти на начало мапируемой области памяти или физический адрес (например, при мапировании регистров устройства при разработке менеджера ресурсов).
    */
    sm = mmap(NULL, sizeof(*sm), PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
    if (sm == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // Инициализация семафора
    if (sem_init(&(sm->sem), 1, 1) == -1) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Записываем начальное значение в разделяемую память
    sm->value = 0;

    // Создание дочернего процесса
    childpid = fork();
    if (childpid == -1) {
        perror("fork");
        return -1;
    }

    if (childpid) {
        // Родительский процесс
        init_logger("parent_log.txt", 1, 1);
        if (process_1()) {
            log_error("process_1()");
            exit(EXIT_FAILURE);
        }
        // Дождаться завершения дочерних процессов
        wait(NULL);
    } else {
        // Дочерний процесс
        init_logger("child_log.txt", 0, 1);
        if (process_2()) {
            log_error("process_2()");
            exit(EXIT_FAILURE);
        }
        if (threads()) {
            log_error("threads()");
            return -1;
        }
    }

    // Завершение
    // Закроем разделяемую область
    close(md);

    // Удалим ее
    shm_unlink(MEM_NAME);

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mut);

    // Уничтожение семафора
    sem_destroy(&sm->sem);

    destroy_logger();
    return (EXIT_SUCCESS);
}

// Два процесса. Используют разделяемую память, синхронизируются при помощи
// семафора. Каждый процесс выполняет различные арифметические действия с одной 
// и той же переменной.

int process_1(void) {
    // Блокируем память при помощи семафора
    sem_wait(&(sm->sem));

    // Работа с разделяемой памятью
    log_info("(1) Previous value: %i\n", sm->value);
    sm->value = 10;
    sm->value += 2;
    sm->value *= 4;
    sm->value -= 15;
    log_info("(1) Result: %i\n", sm->value);

    // Разблокируем
    sem_post(&(sm->sem));

    return 0;
}

int process_2(void) {
    // Берем ключ
    sem_wait(&(sm->sem));

    // Работа с разделяемой памятью
    log_info("(2) Previous value: %i\n", sm->value);
    sm->value = 7;
    sm->value += 5;
    sm->value *= 2;
    sm->value -= 1;
    log_info("(2) Result: %i\n", sm->value);

    // Возвращаем ключ
    sem_post(&(sm->sem));

    return 0;
}

// Функция создания потоков и мьютекс для их синхронизации

int threads(void) {
    pthread_t thid1, thid2; // Идентификаторы потоков

    // Инициализация мьютекса
    if (pthread_mutex_init(&mut, NULL) == -1) {
        printf("pthread_mutex_init error\n");
        log_error("pthread_mutex_init()");
        return -1;
    }

    // Создание потоков
    if (pthread_create(&thid1, NULL, &thread_1, NULL) == -1 || 
        pthread_create(&thid2, NULL, &thread_2, NULL) == -1) {
        log_error("pthread_create()");
        return -1;
    }

    // Дождемся их завершения
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);

    return 0;
}

// Два потока. Синхронизируются между собой при помощи мьютекса. 

void *thread_1(void *arg) {
    // Захват мьютекса
    pthread_mutex_lock(&mut);

    // Выполнение 
    log_info("First lock\n");
    sleep(2);
    log_info("First unlock\n");

    // Освобождение
    pthread_mutex_unlock(&mut);
}

void *thread_2(void *arg) {
    // Захват мьютекса 
    pthread_mutex_lock(&mut);

    // Выполнение 
    log_info("Second lock\n");
    sleep(2);
    log_info("Second unlock\n");

    // Освобождение
    pthread_mutex_unlock(&mut);
}