project (FileSignature)
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Use default cmake build type: Debug")
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(FILE_SIGNATURE_BUILD_TESTS "Build tests" OFF)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -std=c++11 -pthread")

find_package(Boost REQUIRED COMPONENTS system program_options)

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

include_directories(
    ${Boost_INCLUDE_DIRS}
)

file(GLOB SRC_LIST
    ./src/*.h ./src/*.cpp
    ./src/file-reader/*
    ./src/file-writer/*
    ./src/file-chunks-producer/*
    ./src/file-hash-consumer/*
    ./src/file-hash-calculator/*
    ./src/common/*
)

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

set(MODULE_NAME ${PROJECT_NAME})

add_executable(${MODULE_NAME} ${SRC_LIST})

if (FILE_SIGNATURE_BUILD_TESTS)
    add_subdirectory(tests)
    add_library(${MODULE_NAME}_lib ${SRC_LIST})
endif()

target_link_libraries(
    ${PROJECT_NAME}
    ${Boost_LIBRARIES}
)
