#include "reader.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <logger.h>
#include <stdbool.h>
static bool reader_thread_on = true;
static size_t read_number_of_cpu();
static CPU_TAB* read_cpu_create(size_t number);
static int read_cpu(CPU* cpu, FILE* file);
static int read_cpu_tab(CPU_TAB* cpu);

/* MAIN READER FUNCTION */
void* reader_thread(void* arg) {
    Data* data = arg;
    size_t number_of_cpu = read_number_of_cpu();
    if(number_of_cpu < 1) {
        sleep(1); /* sleep sec and try again*/
        number_of_cpu = read_number_of_cpu();
        if(number_of_cpu < 1) {
            abort();
        }
    }
    while (reader_thread_on)
    {
        CPU_TAB* tab = read_cpu_create(number_of_cpu);
        if(read_cpu_tab(tab) == 0) {
            sem_wait(&data->queue_read_empty);
            if(!reader_thread_on) {
                break;
            }
            pthread_mutex_lock(&data->queue_read_mutex);
            queue_push(data->queue_read, tab);
            pthread_mutex_unlock(&data->queue_read_mutex);
            sem_post(&data->queue_read_full);
            LOG_INFO("Pushed readed cpus tab to queue");
        }
        else {
            break;
        }
        sleep(1);
    }

    return NULL;
}
/* END OF MAIN READER FUNCTION */

static size_t read_number_of_cpu() {
    size_t number = 0;
    FILE* file;
    file = fopen("/proc/stat", "r");
    if(!file) {
        LOG_ERROR("Can't open '/proc/stat' file");
    }
    else{
        char buffer[100];
        while(1) {
            fscanf(file, "%s", buffer);
            if( buffer[0] != 'c') {
                break;
            }
            else {
                number++;
            }
            /* each line of cpu have 10 numbers, so skip it */
            for(size_t i = 0; i < 10; i++) {
                int temp;
                ssize_t read = fscanf(file, "%d", &temp);
                if(read == EOF) {
                    number = 0;
                    break;
                }
            }
        }
    }
    fclose(file);
    return number;
}

static int read_cpu(CPU* cpu, FILE* file) {
    fscanf(file, "%s", cpu->name);
    if(cpu->name[0] != 'c') {
        LOG_ERROR("not valid line in /proc/stat file");
        return 1;
    }
    int valRead = fscanf(file, "%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu \n",
                                &cpu->user, &cpu->nice,&cpu->system,
                                &cpu->idle, &cpu->iowait ,&cpu->irq, &cpu->softirq,
                                &cpu->steal, &cpu->guest, &cpu->guest_nice);
    if(valRead == EOF)
        return 1;
    else
        return 0;

}

static CPU_TAB* read_cpu_create(size_t number) {
    CPU_TAB* cpu = malloc(sizeof(CPU_TAB));
    assert(cpu);
    cpu->size = number;
    cpu->cpu_tab = malloc(sizeof(CPU) * number);
    assert(cpu->cpu_tab);
    return cpu;
}

static int read_cpu_tab(CPU_TAB* cpu) {
    assert(cpu);
    assert(cpu->cpu_tab);
    int ret = 0;
    FILE* file = fopen("/proc/stat", "r");
    if(!file) {
        LOG_ERROR("Can't open file \"/proc/stat\"");
        ret = -1;
    }
    else{
        for(size_t i = 0; i < cpu->size; i++) {
            if(read_cpu(&cpu->cpu_tab[i], file) != 0) {
                LOG_ERROR("Can't read cpu %u", i);
            }
        }
        LOG_DEBUG("Successful read from \"/proc/stat\"");
        fclose(file);
    }

    return ret;
}

void reader_free_cpu_tab(void* arg) {
    CPU_TAB* cpu = arg;
    if(!arg) {
        return;
    }
    free(cpu->cpu_tab);
    free(cpu);
}

void reader_end_thread(Data* data) {
    reader_thread_on = false;
    sem_post(&data->queue_read_empty);
}