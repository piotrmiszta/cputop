#include <stdio.h>
#include "queue.h"
#include "logger.h"
#include "reader.h"
#include "types.h"
#include "analyzer.h"
#include <unistd.h>
#include <stdlib.h>
#include "printer.h"

void init(Data* data);
void destroy(Data* data);

int main() {
    char filename[] = "log.txt";
    logger_init();
    Data data;
    init(&data);
    pthread_t readerThread;
    pthread_t loggerThread;
    pthread_t analyzerThread;
    pthread_t printerThread;

    pthread_create(&loggerThread, NULL, logger_thread, filename);
    pthread_create(&readerThread, NULL, reader_thread, &data);
    pthread_create(&analyzerThread, NULL, analyzer_thread, &data);
    pthread_create(&printerThread, NULL, printer_thread, &data);

    sleep(10);

    reader_end_thread(&data);
    analyzer_end_thread(&data);
    printer_end_thread(&data);

    logger_close();
    pthread_join(loggerThread, NULL);
    pthread_join(analyzerThread, NULL);
    pthread_join(readerThread, NULL);
    pthread_join(printerThread, NULL);
    destroy(&data);
    //logger_close();
}

void init(Data* data){
    data->queue_read = queue_create(reader_free_cpu_tab);
    data->queue_analysis = queue_create(free);
    pthread_mutex_init(&data->queue_read_mutex, NULL);
    sem_init(&data->queue_read_empty, 0, 10);
    sem_init(&data->queue_read_full, 0, 0);

    pthread_mutex_init(&data->queue_analyzer_mutex, NULL);
    sem_init(&data->queue_analyzer_empty, 0, 10);
    sem_init(&data->queue_analyzer_full, 0, 0);
}

void destroy(Data* data) {
    queue_destroy(data->queue_read);
    queue_destroy(data->queue_analysis);
    pthread_mutex_destroy(&data->queue_read_mutex);
    sem_destroy(&data->queue_read_empty);
    sem_destroy(&data->queue_read_full);
}