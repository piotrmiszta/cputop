#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

void test1(void** state) {
  assert_int_equal(1, 1);
}





int main(void)
{
  const struct CMUnitTest tests[] = {
    unit_test(test1),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
