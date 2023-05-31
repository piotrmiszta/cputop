#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "queue.h"
void fre(void* arg) {
    free(arg);
}

void queue_create_test(void** state) {
    Queue* que = queue_create(fre);
    for(int i = 0; i < 10; i++) {
        int* k = malloc(sizeof(int));
        *k = i;
        queue_push(que, k);
    }
    for(int i = 0; i < 10; i++) {
        int* s = (int*)queue_pop(que);
        assert_int_equal(i, *(s));
        free(s);
    }
    for(int i = 0; i < 100; i++) {
        int* k = malloc(sizeof(int));
        *k = i;
        queue_push(que, k);
    }
    for(int i = 0; i < 100; i++) {
        int* s = (int*)queue_pop(que);
        assert_int_equal(i, *(s));
        free(s);
    }
    for(int i = 0; i < 10; i++) {
        int* k = malloc(sizeof(int));
        *k = i;
        queue_push(que, k);
    }
    for(int i = 0; i < 10; i++) {
        int* s = (int*)queue_pop(que);
        assert_int_equal(i, *(s));
        free(s);
    }
    for(int i = 0; i < 10; i++) {
        int* k = malloc(sizeof(int));
        *k = i;
        queue_push(que, k);
    }
    queue_destroy(que);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    unit_test(queue_create_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
