cmake_minimum_required(VERSION 3.29)
set(CMAKE_C_COMPILER /usr/bin/gcc) 
project(Snekken C)
# Set C standard
set(CMAKE_C_STANDARD 11)
# Add your source files
set(SOURCES
    src/main.c
    # Add more source files here if you have multiple
)

# Create executable
add_executable(Snekken ${SOURCES})

# Include current directory
target_include_directories(Snekken PRIVATE ${CMAKE_SOURCE_DIR})

# Link raylib library
target_link_libraries(Snekken "/usr/local/lib/libraylib.a")

# If you are on Linux, you might need to link additional libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(Snekken m dl pthread rt)
endif()
