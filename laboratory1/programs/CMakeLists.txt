foreach(program program_array program_c_string program_file)
    add_executable(${program} ${program}.cpp)
    target_link_libraries(${program} PRIVATE string_processing)
    target_compile_options(${program} PRIVATE
        -Wall -Wextra -Wpedantic
    )
endforeach()
