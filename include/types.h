#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>

typedef unsigned int ui;

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
    CPU cpu0;
    CPU* cpu_tab;
    size_t size;
}CPU_TAB;

#endif //TYPES_H