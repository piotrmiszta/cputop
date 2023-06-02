#ifndef ANALYZER_H
#define ANALYZER_H
#include "types.h"

void* analyzer_thread(void* arg);
void analyzer_end_thread(Data* data);

#endif //ANALYZER