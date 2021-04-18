cmake_minimum_required(VERSION 3.16.3)

# set(CMAKE_BUILE_TYPE "Debug")

# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_BUILD_TYPE Debug)

project(demo)

set(SOURCES
	src/servo.cpp
	src/PCA9685.cpp
    src/test.cpp
)

add_executable(demo ${SOURCES})

target_include_directories(demo
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(demo ${WIRINGPI_LIBRARIES})






