cmake_minimum_required(VERSION 3.16)

project(psx)

set(CXX_STANDARD c++17)

set(SOURCES
    src/main.cpp
    src/mem/bus.cpp
    src/cpu/cpu.cpp
    src/cpu/instruction.cpp
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMAND ON)

add_executable(psx ${SOURCES})
set(TARGET_NAME psx)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)