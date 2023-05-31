set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h")

file(GLOB src "${CMAKE_SOURCE_DIR}/src/*.c")

add_library(cputoplib STATIC    ${HEADER_LIST}
                                ${src})


target_include_directories(cputoplib PUBLIC    ${INCLUDE_DIR}/)
