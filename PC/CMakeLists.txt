cmake_minimum_required(VERSION 3.28)
project(UART)

set(CMAKE_CXX_STANDARD 17)

add_executable(UART main.cpp
        device.cpp
        device.h
        midi.cpp
        midi.h)

target_link_libraries(UART winmm)