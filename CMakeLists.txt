cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(FFT-Test)

# Set C++ version 20
set(CMAKE_CXX_STANDARD 20)

# Load dependencies
include(cmake/Dependencies.cmake)

# Create the executable
add_executable(FFT-Test FFT-Test.cpp Plot.cpp Data.cpp)

# We have assets to copy to the build/install directory
add_custom_command(TARGET FFT-Test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/
        $<TARGET_FILE_DIR:FFT-Test>/assets)

# Make sure to install

# Adds external dependencies.
add_ext_dependencies(FFT-Test)