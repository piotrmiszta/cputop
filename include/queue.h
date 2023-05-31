#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
typedef struct Queue Queue;

Queue* queue_create(void(*free_data)(void* data));
void queue_destroy(Queue* que);
void queue_push(Queue* restrict que, void* data);
void* queue_pop(Queue* restrict que);
bool queue_empty(Queue* restrict que);
#endif //QUEUE_H