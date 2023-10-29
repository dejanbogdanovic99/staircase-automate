
cmake_minimum_required(VERSION 3.4)

project(staircase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Werror -fno-exceptions)

find_package(GTest REQUIRED)

enable_testing()

set(STARICASE_LIB ${PROJECT_NAME})

add_library(${STARICASE_LIB} STATIC
    src/staircase/BasicLight.cxx
    src/staircase/Moving.cxx
    src/staircase/ProximitySensor.cxx
    src/staircase/StaircaseLooper.cxx
)

target_include_directories(${STARICASE_LIB}
    PUBLIC
        include
)

add_subdirectory(tests)