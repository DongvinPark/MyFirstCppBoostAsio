# 현재의 CMakeLists.txt의 내용은  WindowOS Visual Stuido IDE용이다.
# Linux와 M Series Chip MacOS를 위한 CMakeLists.txt는 README.md에 기재돼 있다.
cmake_minimum_required(VERSION 3.29)
project(MyBoostAsioProject)

set(CMAKE_CXX_STANDARD 20)

# Add Vcpkg toolchain file if using Vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(Boost REQUIRED COMPONENTS system)

add_executable(MyBoostAsioProject src/main.cpp "src/dto/Res.cpp")

# Link Boost.System and Winsock (ws2_32)
target_link_libraries(MyBoostAsioProject Boost::system ws2_32)