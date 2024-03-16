
cmake_minimum_required(VERSION 3.6)

project(staircase VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Werror -fno-exceptions)

option(BUILD_SHARED "whether to build the shared library" OFF)
option(BUILD_STATIC "whether to build the static library" ON)
option(BUILD_TESTS "whether to build tests" ON)

if(NOT BUILD_STATIC AND NOT BUILD_SHARED)
    message(FATAL_ERROR "Cannot build without building libraries")
endif()

set(LIGHTS_NUM 8 CACHE STRING "Number of lights in the system")
set(DEFAULT_ON_PERIOD 3000 CACHE STRING "Default number of milliseconds for light on state")
set(DEBOUNCE_PERIOD 300 CACHE STRING "Debounce period for proximity sensor")
set(MAX_MOVINGS 3 CACHE STRING "Number of one-direction movings allowed")
set(MOVING_FINISH_DELTA 2000 CACHE STRING "Number of milliseconds around end which is considered valid")

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

    target_compile_definitions(${STAIRCASE_LIB_STATIC}
        PUBLIC LIGHTS_NUM=${LIGHTS_NUM}
        PUBLIC DEFAULT_ON_PERIOD=${DEFAULT_ON_PERIOD}
        PUBLIC DEBOUNCE_PERIOD=${DEBOUNCE_PERIOD}
        PUBLIC MAX_MOVINGS=${MAX_MOVINGS}
        PUBLIC MOVING_FINISH_DELTA=${MOVING_FINISH_DELTA})
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

    target_compile_definitions(${STAIRCASE_LIB_SHARED}
        PUBLIC LIGHTS_NUM=${LIGHTS_NUM}
        PUBLIC DEFAULT_ON_PERIOD=${DEFAULT_ON_PERIOD}
        PUBLIC DEBOUNCE_PERIOD=${DEBOUNCE_PERIOD}
        PUBLIC MAX_MOVINGS=${MAX_MOVINGS}
        PUBLIC MOVING_FINISH_DELTA=${MOVING_FINISH_DELTA})
endif()


if(BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()
    add_subdirectory(tests)
endif()