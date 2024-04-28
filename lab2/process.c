#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "../include/logger.h"

void main()
{
    pid_t pid;
    pid = fork();
    init_logger("process.txt", 1, 1);

    switch(pid)
    {
        case -1:
            log_error("fork error");
            destroy_logger();
            exit(1);

        case 0:
            log_info("Дочерний процесс: pid = %d, ppid = %d", getpid(), getppid());
            break;

        default:
            log_info("Родительский процесс: pid = %d", getpid());
            break;
    }
    destroy_logger();
    return 0;
}