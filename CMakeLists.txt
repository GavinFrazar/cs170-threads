cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(HEADERS
  threads.h)

set(SOURCES
  threads.c)

project("threads" C)
set(LIBRARY_NAME threads_lib)
add_library(${LIBRARY_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_options(${LIBRARY_NAME} PUBLIC -m32)
include_directories(".")
add_subdirectory("p2_basic_tests")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") 
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -m32")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -m32")
add_executable(test main.c) 
target_link_libraries(test ${LIBRARY_NAME})
target_compile_options(test PRIVATE -m32)

