cmake_minimum_required(VERSION 3.10)
project(debugger)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and include directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Add include directory
include_directories(${INCLUDE_DIR})

# Collect source files
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cc")

# Debug output
message(STATUS "Source files found: ${SOURCES}")

# Fail with a clear message if no source files were found
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ${SRC_DIR}")
endif()

# Create executable
add_executable(debugger ${SOURCES})
