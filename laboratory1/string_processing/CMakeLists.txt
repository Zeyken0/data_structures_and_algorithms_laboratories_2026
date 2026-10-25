add_library(string_processing STATIC
    src/array_string.cpp
    src/c_string.cpp
    src/file_string.cpp
    src/string_utils.cpp
)

target_include_directories(string_processing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(string_processing PRIVATE
    -Wall -Wextra -Wpedantic
)
