cmake_minimum_required(VERSION 3.10)
project(Snekken C)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Include raylib
include_directories(${CMAKE_SOURCE_DIR})

# Add your source files
set(SOURCES
    src/main.c
    # Add more source files here if you have multiple
)

# Create executable
add_executable(Snekken ${SOURCES})

# Link raylib library
target_link_libraries(Snekken "${CMAKE_SOURCE_DIR}/libraylib.a")

# If you are on Linux, you might need to link additional libraries
if(UNIX AND NOT APPLE)
  target_link_libraries(Snekken m dl pthread rt)
endif()

