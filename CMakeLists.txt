cmake_minimum_required(VERSION 3.29)
set(CMAKE_C_COMPILER /usr/bin/gcc) 
project(Snekken C)
set(CMAKE_C_STANDARD 11)
set(SOURCES
    src/main.c
)

add_executable(Snekken ${SOURCES})

target_include_directories(Snekken PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(Snekken "/usr/local/lib/libraylib.a")

if(UNIX AND NOT APPLE)
    target_link_libraries(Snekken m dl pthread rt)
endif()
