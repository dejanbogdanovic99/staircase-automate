
set(STAIRCASE_TESTS ${PROJECT_NAME}_tests)
set(TESTS all-tests)

add_executable(${STAIRCASE_TESTS}
    src/BasicLightTests.cxx
    src/ClippedSquaredMovingDurationCalculatorTests.cxx
    src/MovingTests.cxx
    src/MTAMovingTimeFilterTests.cxx
    src/ProximitySensorTests.cxx
    src/StaircaseLooperTests.cxx
)

target_include_directories(${STAIRCASE_TESTS}
    PRIVATE
        include
)

if(BUILD_STATIC)
    set(STAIRCASE_LIB ${STAIRCASE_LIB_STATIC})
elseif(BUILD_SHARED)
    set(STAIRCASE_LIB ${STAIRCASE_LIB_SHARED})
endif()

target_link_libraries(${STAIRCASE_TESTS}
    PUBLIC
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
        ${STAIRCASE_LIB}
)

add_test(${TESTS} ${STAIRCASE_TESTS})