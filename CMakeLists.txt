
cmake_minimum_required(VERSION 3.4)

project(staircase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Werror -fno-exceptions)

option(BUILD_SHARED "whether to build the shared library" OFF)
option(BUILD_STATIC "whether to build the static library" ON)
option(BUILD_TESTS "whether to build tests" ON)

if(NOT BUILD_STATIC AND NOT BUILD_SHARED)
    message(FATAL_ERROR "Cannot build without building libraries")
endif()

set(STAIRCASE_LIB_SRCS
    src/staircase/BasicLight.cxx
    src/staircase/Moving.cxx
    src/staircase/ProximitySensor.cxx
    src/staircase/StaircaseLooper.cxx
)

if (BUILD_STATIC)
    set(STAIRCASE_LIB_STATIC ${PROJECT_NAME}_s)

    add_library(${STAIRCASE_LIB_STATIC} STATIC
        ${STAIRCASE_LIB_SRCS}
    )

    target_include_directories(${STAIRCASE_LIB_STATIC}
        PUBLIC
            include
    )
endif()

if (BUILD_SHARED)
    set(STAIRCASE_LIB_SHARED ${PROJECT_NAME})

    add_library(${STAIRCASE_LIB_SHARED} SHARED
        ${STAIRCASE_LIB_SRCS}
    )

    target_include_directories(${STAIRCASE_LIB_SHARED}
        PUBLIC
            include
    )
endif()


if(BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()
    add_subdirectory(tests)
endif()