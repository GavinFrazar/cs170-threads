set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") 
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -m32")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -m32")

add_executable(basic_test_1 basic_1.c)
target_link_libraries(basic_test_1 ${LIBRARY_NAME})
target_compile_options(basic_test_1 PRIVATE -m32)

add_executable(basic_test_2 basic_2.c) 
target_link_libraries(basic_test_2 ${LIBRARY_NAME})
target_compile_options(basic_test_2 PRIVATE -m32)

add_executable(basic_test_3 basic_3.c) 
target_link_libraries(basic_test_3 ${LIBRARY_NAME})
target_compile_options(basic_test_3 PRIVATE -m32)

add_executable(basic_test_4 basic_4.c) 
target_link_libraries(basic_test_4 ${LIBRARY_NAME})
