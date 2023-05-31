#include <stdio.h>
#include "queue.h"
#include <logger.h>
#include <pthread.h>
#include <stdarg.h>

int main() {
    logger_init("log.txt");

    logger_close();
}