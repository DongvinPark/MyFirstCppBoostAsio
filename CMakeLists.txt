cmake_minimum_required(VERSION 3.10)

# Project name and version
project(MyBoostAsioProject VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the Boost library
find_package(Boost REQUIRED COMPONENTS system)

# Include the directories for header files
include_directories(${Boost_INCLUDE_DIRS} include src)

# Collect all source files from subdirectories
file(GLOB_RECURSE SOURCES
    src/main.cpp
)

# Create the executable
add_executable(MyBoostAsioProject ${SOURCES})

# Link Boost libraries to the project
target_link_libraries(MyBoostAsioProject ${Boost_LIBRARIES})