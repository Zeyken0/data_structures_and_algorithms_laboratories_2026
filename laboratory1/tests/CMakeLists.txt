set(CATCH2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/catch2)

add_executable(string_tests
    test_array_string.cpp
    test_c_string.cpp
    test_file_string.cpp
)

target_include_directories(string_tests PRIVATE ${CATCH2_INCLUDE_DIR})

target_link_libraries(string_tests PRIVATE string_processing)

target_compile_options(string_tests PRIVATE
    -Wall -Wextra -Wpedantic
)

add_test(NAME string_tests COMMAND string_tests)
