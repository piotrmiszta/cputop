#include "printer.h"
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include "logger.h"
static bool printer_thread_on = true;
static void printer_print(AnalyzerTab* tab);
void* printer_thread(void* arg) {
    Data* data = arg;
    while(1) {
        sem_wait(&data->queue_analyzer_full);
        if(!printer_thread_on) {
            break;
        }
        pthread_mutex_lock(&data->queue_analyzer_mutex);
        AnalyzerTab* to_print = queue_pop(data->queue_analysis);
        pthread_mutex_unlock(&data->queue_analyzer_mutex);
        sem_post(&data->queue_analyzer_empty);
        LOG_DEBUG("PRINTER: pop from analyzer queue");

        printer_print(to_print);
        free(to_print->tab);
        free(to_print);
    }
    return NULL;
}

void printer_end_thread(Data* data) {
    printer_thread_on = false;
    sem_post(&data->queue_analyzer_full);
}

static void printer_print(AnalyzerTab* tab) {
    for(size_t i = 0; i < tab->size; i++) {
        printf("[] cpu%lu = %f \n", i, tab->tab[i]);
    }
}
