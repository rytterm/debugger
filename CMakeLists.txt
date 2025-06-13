cmake_minimum_required(VERSION 3.10)
project(debugger C CXX)  # Enable both C and C++ languages

# Set C++ and C standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")

# Include directories (public headers)
include_directories(${INCLUDE_DIR} ${LIB_DIR})

# Collect source files: only .c and .cc files from src and lib
file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/*.c"
    "${SRC_DIR}/*.cc"
    "${LIB_DIR}/*.c"
    "${LIB_DIR}/*.cc"
)

message(STATUS "Source files found: ${SOURCES}")

if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ${SRC_DIR} or ${LIB_DIR}")
endif()

add_executable(debugger ${SOURCES})

# Optionally: Use target_include_directories for better practice
target_include_directories(debugger PRIVATE ${INCLUDE_DIR} ${LIB_DIR})
