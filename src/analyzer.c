#include "analyzer.h"
#include "logger.h"
#include "reader.h"
#include <stdlib.h>
#include <assert.h>
static bool analyzer_thread_on = true;
static AnalyzerTab* analyzer_calculate(CPU_TAB* const prev, const CPU_TAB* const this);
void* analyzer_thread(void* arg) {
    Data* data = arg;
    CPU_TAB* prev = NULL;
    CPU_TAB* this = NULL;
    while(1) {
        sem_wait(&data->queue_read_full);
        if(!analyzer_thread_on) {
            break;
        }
        pthread_mutex_lock(&data->queue_read_mutex);

        this = queue_pop(data->queue_read);

        pthread_mutex_unlock(&data->queue_read_mutex);
        sem_post(&data->queue_read_empty);
        LOG_INFO("Poped from queue_read");
        /* calculate how much proc are used */
        if(prev == NULL) {
            /* continue */
            prev = this;
            this = NULL;
        }
        else {
            AnalyzerTab* percent_tab = analyzer_calculate(prev, this);
            /* push percent tab to analyzer queue */
            sem_wait(&data->queue_analyzer_empty);
            if(!analyzer_thread_on) {
                break;
            }
            pthread_mutex_lock(&data->queue_analyzer_mutex);
            queue_push(data->queue_analysis, percent_tab);
            LOG_DEBUG("Analyzer: push to analyzer queue");
            pthread_mutex_unlock(&data->queue_analyzer_mutex);
            sem_post(&data->queue_analyzer_full);
            reader_free_cpu_tab(prev);
            prev = this;
            this = NULL;
        }
    }
    reader_free_cpu_tab(prev);
    reader_free_cpu_tab(this);
    return NULL;
}

void analyzer_end_thread(Data* data) {
    analyzer_thread_on = false;
    sem_post(&data->queue_read_full);
    sem_post(&data->queue_analyzer_empty);

}

static AnalyzerTab* analyzer_calculate(CPU_TAB* const prev, const CPU_TAB* const this) {
    double* tab = malloc(sizeof(double) * prev->size);
    AnalyzerTab* que_element = malloc(sizeof(AnalyzerTab));
    que_element->size = prev->size;
    assert(tab);
    assert(que_element);
    for( int i = 0; i < prev->size; i++) {

        ui prevIdle = prev->cpu_tab[i].idle+prev->cpu_tab[i].iowait;
        ui prevNonIdle =    \
            prev->cpu_tab[i].user + prev->cpu_tab[i].nice +     \
            prev->cpu_tab[i].system + prev->cpu_tab[i].irq + prev->cpu_tab[i].steal;
        ui idle = this->cpu_tab[i].idle+this->cpu_tab[i].iowait;
        ui nonIdle =        \
         this->cpu_tab[i].user + this->cpu_tab[i].nice + this->cpu_tab[i].system +  \
         this->cpu_tab[i].irq + this->cpu_tab[i].steal;
        ui prevTotal = prevIdle + prevNonIdle;
        ui total = idle+nonIdle;
        double totald = (double)total - (double)prevTotal;
        double idled = (double)idle - (double)prevIdle;
        double cpuUsage;
        if(totald) {
            cpuUsage = (1000*(totald - idled) / (totald) + 1)/10;
            tab[i] = cpuUsage;
        }
    }
    que_element->tab = tab;
    return que_element;
}