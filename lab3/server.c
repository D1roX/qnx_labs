#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include "../include/logger.h"

void server(void) {
    int rcvid;
    int chid;
    char message[256];
    char response[512];

    log_info("Сервер начал работу");

    chid = ChannelCreate(0);
    log_info("id канала: %d", chid);

    log_info("Pid: %d", getpid());

    while (1) {
        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        log_info("Полученное сервером сообщение: '%s'", message);
        snprintf(response, sizeof(response), "Это ответ сервера на сообщение '%s'", message);
        MsgReply(rcvid, EOK, response, sizeof(response));

        log_info("Отправленное сервером сообщение: '%s'", response);
    }
}

int main(void) {
    init_logger("server.txt", 0, 1);
    log_info("Сервер");
    server();
    destroy_logger();
    return (1);
}