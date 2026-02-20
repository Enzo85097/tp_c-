cmake_minimum_required(VERSION 3.16)
project(tp_note_cpp2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tp_note_cpp2
    src/main.cpp
    src/Database.cpp
    src/AuthService.cpp
    src/StudentService.cpp
)

target_include_directories(tp_note_cpp2 PRIVATE include)
target_link_libraries(tp_note_cpp2 PRIVATE sqlite3)
