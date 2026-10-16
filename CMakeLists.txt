cmake_minimum_required(VERSION 3.16)

project(psx LANGUAGES CXX)

set(SOURCES
    src/main.cpp
    src/mem/bus.cpp
    src/cpu/cpu.cpp
    src/cpu/instruction.cpp
)

add_executable(psx ${SOURCES})

set_target_properties(psx PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(psx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)