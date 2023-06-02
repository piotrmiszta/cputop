#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>
#include "queue.h"
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX_READER_QUEUE    10

typedef long long unsigned int ui;

typedef struct CPU {
    char name[7]; // CPUXX
    ui user;    //process executing in user mode
    ui nice;    //process executing with niced priority
    ui system;  //process in kernel mode
    ui idle;    //idling
    ui iowait;  //IO requests
    ui irq;     //interrupt request
    ui softirq; //servicing softirq
    ui steal;   //virtual envi
    ui guest;   //virtual CPU
    ui guest_nice;  //virtual CPU with niced priority
}CPU;

typedef struct CPU_TAB{
    CPU* cpu_tab;
    size_t size;
}CPU_TAB;

typedef struct AnalyzerTab{
    double* tab;
    size_t size;
}AnalyzerTab;

typedef struct Data{
    Queue* queue_read;      //reader read here data
    Queue* queue_analysis;  //after analaizer

    sem_t queue_read_full;
    sem_t queue_read_empty;

    sem_t queue_analyzer_full;
    sem_t queue_analyzer_empty;

    pthread_mutex_t queue_read_mutex;
    pthread_mutex_t queue_analyzer_mutex;

}Data;

#endif //TYPES_H