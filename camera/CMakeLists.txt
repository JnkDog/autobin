cmake_minimum_required(VERSION 3.16.3)

# If you don't want debug info, you can remove the below line
set(CMAKE_BUILD_TYPE Debug)

project(camera)

set(SOURCES
	src/client.cpp
)

add_executable(camera ${SOURCES})

target_include_directories(camera
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

find_package(Threads)

target_link_libraries(camera ${CMAKE_THREAD_LIBS_INIT})