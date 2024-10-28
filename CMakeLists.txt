cmake_minimum_required(VERSION 3.29)
project(MyBoostAsioProject)

set(CMAKE_CXX_STANDARD 20)

# Add Vcpkg toolchain file if using Vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(Boost REQUIRED COMPONENTS system)

add_executable(MyBoostAsioProject src/main.cpp "src/dto/res_second.cpp")

# Link Boost.System and Winsock (ws2_32)
target_link_libraries(MyBoostAsioProject Boost::system ws2_32)