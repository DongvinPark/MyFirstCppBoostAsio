cmake_minimum_required(VERSION 3.16)

# Project name and version
project(MyBoostAsioProject VERSION 1.0)

# Set C++ standard to 17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add platform-specific settings
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	# Set toolchain for vcpkg (Windows only)
	set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

	# Find Boost using vcpkg
	find_package(Boost REQUIRED COMPONENTS system)

	# Additional libraries required on Windows
	set(PLATFORM_LIBS ws2_32)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	# Find Boost using Homebrew (macOS only)
	find_package(Boost REQUIRED)

	# No additional libraries required for macOS
	set(PLATFORM_LIBS "")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# Find Boost
	find_package(Boost REQUIRED)

	# Link pthread library for Linux
	set(PLATFORM_LIBS pthread)
endif()

# Include directories
include_directories(${Boost_INCLUDE_DIRS} src)

add_executable(
		MyBoostAsioProject src/main.cpp
		"src/dto/Res.h"
		"src/dto/Res.cc"
)

# Link Boost libraries and platform-specific libraries
target_link_libraries(MyBoostAsioProject ${Boost_LIBRARIES} ${PLATFORM_LIBS})