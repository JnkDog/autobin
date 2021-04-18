cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_BUILD_TYPE Debug)

project(midware)

set(SOURCES
	../hardware/src/servo.cpp
	../hardware/src/PCA9685.cpp
	src/midware.cpp
)

add_executable(midware ${SOURCES})

target_include_directories(midware
    PRIVATE 
	${PROJECT_SOURCE_DIR}/include
	../hardware/include
)

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(midware ${WIRINGPI_LIBRARIES} jsoncpp)
