#include "logger.h"

void test_func(int x) {
    switch (x) {
        case 0:
            log_debug("x = 0");
            break;
        case 1:
            log_warn("x = 1");
            break;
        case 2:
            log_info("x = 2");
            break;
        case 3:
            log_error("x = 3");
            break;
        default:
            break;
    }
}

int main() {
    init_logger(DEBUG, "log.txt", 0, 1);
    log_error("Уровень логирования ERROR. Рохлин Д.Е., Сидорова Ю. Д.И903Б");
    log_warn("Уровень логирования WARNING. Рохлин Д.Е., Сидорова Ю. Д. И903Б");
    log_info("Уровень логирования INFO. Рохлин Д.Е., Сидорова Ю. Д. И903Б");
    log_debug("Уровень логирования DEBUG. Рохлин Д.Е., Сидорова Ю. Д. И903Б");
    test_func(0);
    test_func(1);
    test_func(2);
    test_func(3);
    return 0;
}