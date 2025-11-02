cmake_minimum_required(VERSION 3.10)

project(MAVCOMM
    VERSION 1.0.0
    DESCRIPTION "MAVLink communications library"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MAVCOMM_LIBS
    SOCKLIB # socket library
)

target_compile_definitions(MAVCOMM
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG> # define DEBUG macro when build in debug mode
)

add_library(MAVCOMM
    STATIC
        mavcomm.cpp
)

target_link_libraries(MAVCOMM 
    PRIVATE 
        ${MAVCOMM_LIBS}
)

target_include_directories(MAVCOMM
    PRIVATE
        "${PROJECT_BINARY_DIR}"
)


