#include <stdio.h>
#include "queue.h"
#include "logger.h"
#include <pthread.h>
#include <stdarg.h>
#include "reader.h"
#include "types.h"
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

int main() {

    char* filename = "log.txt";
    logger_init();
    //Data* data = malloc(sizeof(data));
    //pthread_t readerThread;
    //pthread_create(&readerThread, NULL, reader_thread, data);
    //pthread_join(readerThread, NULL);
    //logger_close();
    pthread_t loggerThread;

    pthread_create(&loggerThread, NULL, logger_thread, filename);
    LOG_ERROR("TEST DEBUGU ERROR");
    LOG_INFO("TEST DEBUGU ERROR");
    LOG_DEBUG("TEST DEBUGU ERROR");
    LOG_FATAL("TEST DEBUGU ERROR");
    LOG_WARNING("TEST DEBUGU ERROR");
    logger_close();
    pthread_join(loggerThread, NULL);
}