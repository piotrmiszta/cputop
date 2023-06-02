#ifndef READER_H
#define READER_H
#include "types.h"
void* reader_thread(void* arg);
void reader_free_cpu_tab(void* arg);
void reader_end_thread(Data* data);
#endif // READER_H