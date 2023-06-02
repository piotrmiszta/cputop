#ifndef PRINTER_H
#define PRINTER_H
#include "types.h"

void* printer_thread(void* arg);
void printer_end_thread(Data* data);
#endif // PRINTER_H