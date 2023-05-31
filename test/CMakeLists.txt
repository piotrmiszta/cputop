set(TEST_DIR "${CMAKE_SOURCE_DIR}/test/test_cases")

list(APPEND TEST_LIBS "cmocka")
list(APPEND TEST_LIBS "cputoplib")
set(CMAKE_C_STANDARD 17)
list(APPEND TEST_DIRS "${CMOCKA_INCLUDE_DIRS}")
list(APPEND TEST_DIRS "${CMAKE_SOURCE_DIR}/src")
include_directories(${CMAKE_SOURCE_DIR}/src)
add_cmocka_test(queue_test
  SOURCES ${TEST_DIR}/queue_test.c
  COMPILE_OPTIONS ${DEFAULT_C_COMPILE_FLAGS}
  LINK_OPTIONS ${DEFAULT_LINK_FLAGS}
  LINK_LIBRARIES ${TEST_LIBS})

add_cmocka_test_environment(queue_test)


target_include_directories(queue_test PUBLIC "${TEST_DIRS}")

