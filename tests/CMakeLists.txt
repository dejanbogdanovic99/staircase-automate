
set(STAIRCASE_TEST_BINARY ${PROJECT_NAME}_bin)
set(TESTS all-tests)

add_executable(${STAIRCASE_TEST_BINARY}
    src/BasicLightTests.cxx
    src/MovingTests.cxx
    src/ProximitySensorTests.cxx
    src/StaticDequeTests.cxx
)

target_include_directories(${STAIRCASE_TEST_BINARY}
    PRIVATE
        include
)

target_link_libraries(${STAIRCASE_TEST_BINARY}
    PUBLIC
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
        ${STARICASE_LIB}
)

add_test(${TESTS} ${STAIRCASE_TEST_BINARY})