#ifndef LOGGER_H
#define LOGGER_H
#include <stddef.h>
#include <pthread.h>
typedef enum LogerType{
    LOGGER_TYPE_DEBUG,
    LOGGER_TYPE_INFO,
    LOGGER_TYPE_WARNING,
    LOGGER_TYPE_ERROR,
    LOGGER_TYPE_FATAL,
}LogerType;

void logger_init(void);
void logger_close(void);

void* logger_thread(void* arg);
void logger_log__(LogerType type,
    const char* file,
    const char* func,
    size_t line,
    const char* fmt, ...);

#define logger_log_(type, ...)  \
    logger_log__(type, __FILE__, __func__, __LINE__, __VA_ARGS__)

#define LOG_DEBUG(...)      logger_log_(LOGGER_TYPE_DEBUG, __VA_ARGS__)
#define LOG_ERROR(...)      logger_log_(LOGGER_TYPE_ERROR, __VA_ARGS__)
#define LOG_FATAL(...)      logger_log_(LOGGER_TYPE_FATAL, __VA_ARGS__)
#define LOG_INFO(...)       logger_log_(LOGGER_TYPE_INFO, __VA_ARGS__)
#define LOG_WARNING(...)    logger_log_(LOGGER_TYPE_WARNING, __VA_ARGS__)

#endif