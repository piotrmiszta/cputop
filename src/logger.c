#include "logger.h"
#include "types.h"
#include <stdio.h>
#include <stdarg.h>
#include <queue.h>
#include <semaphore.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define LOGGER_STDOUT

#define RED                 "\x1B[31m"
#define GREEN               "\x1B[32m"
#define PURPLE              "\x1B[35m"
#define BLUE                "\x1B[36m"
#define RESET               "\x1B[0m"

typedef struct LoggerMsg{
    char file[50];
    char func[50];
    size_t line;
    LogerType logType;
    char msg[1024];
}LoggerMsg;

static Queue* que;
static pthread_mutex_t mutex;
static sem_t empty;
static sem_t full;
static bool thread_run = true;

static void logger_clean(void);

void logger_init(void) {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 10);
    sem_init(&full, 0, 0);
    que = queue_create(free);
}
static void logger_print(FILE* stream);

void* logger_thread(void* arg) {
    assert(arg);
    char* filename = arg;
    FILE* file = fopen(filename, "w");
    assert(file);
    while(thread_run) {
        sem_wait(&full);
        if(thread_run == false){
            break;
        }
        pthread_mutex_lock(&mutex);
        logger_print(file);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    /* if braek loop above, end printing by check que and fclose file */
    while(!queue_empty(que)) {
        pthread_mutex_lock(&mutex);
        logger_print(file);
        pthread_mutex_unlock(&mutex);
    }
    fclose(file);
    logger_clean();
    return NULL;
}

static void logger_print(FILE* stream) {

    LoggerMsg* data = queue_pop(que);
    switch (data->logType)
    {
    case LOGGER_TYPE_DEBUG:
        fprintf(stream, GREEN"[ DEBUG ] " RESET);
        #ifdef LOGGER_STDOUT
        fprintf(stdout, GREEN"[ DEBUG ] " RESET);
        #endif
        break;
    case LOGGER_TYPE_INFO:
        fprintf(stream, PURPLE"[ INFO ] " RESET);
        #ifdef LOGGER_STDOUT
        fprintf(stdout, PURPLE"[ INFO ] " RESET);
        #endif
        break;
    case LOGGER_TYPE_WARNING:
        fprintf(stream, BLUE"[ WARNING ] " RESET);
        #ifdef LOGGER_STDOUT
        fprintf(stdout, BLUE"[ WARNING ] " RESET);
        #endif
        break;
    case LOGGER_TYPE_ERROR:
        fprintf(stream, RED"[ ERROR ] " RESET);
        #ifdef LOGGER_STDOUT
        fprintf(stdout, RED"[ ERROR ] " RESET);
        #endif
        break;
    case LOGGER_TYPE_FATAL:
        fprintf(stream, RED"[ FATAL ] " RESET);
        #ifdef LOGGER_STDOUT
        fprintf(stdout, RED"[ FATAL ] " RESET);
        #endif
        break;
    default:
        fprintf(stream, "[ DEFAULT ] ");
        #ifdef LOGGER_STDOUT
        fprintf(stdout, "[ DEFAULT ] ");
        #endif
        break;
    }
    fprintf(stream, "%s %s: %lu  --  %s\n", data->file, data->func, data->line, data->msg);
    #ifdef LOGGER_STDOUT
    fprintf(stdout, "%s %s: %lu  --  %s\n", data->file, data->func, data->line, data->msg);
    #endif
    free(data);
}

void logger_log__(LogerType type,
    const char* file,
    const char* func,
    size_t line,
    const char* fmt, ...) {
    LoggerMsg* msg = calloc(1, sizeof(LoggerMsg));
    assert(msg);
    msg->logType = type;
    strcpy(msg->file, file);
    strcpy(msg->func, func);
    va_list list;
    va_start(list, fmt);
    vsnprintf(msg->msg, 1024, fmt, list);
    va_end(list);

    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    queue_push(que, msg);
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}


void logger_close(void) {
    thread_run = false;
    sem_post(&full);
}

static void logger_clean(void) {
    queue_destroy(que);
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
}
