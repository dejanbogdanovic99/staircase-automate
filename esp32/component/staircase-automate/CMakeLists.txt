idf_component_register(
    SRCS
        ../../../src/staircase/BasicLight.cxx
        ../../../src/staircase/ClippedSquaredMovingDurationCalculator.cxx
        ../../../src/staircase/Moving.cxx
        ../../../src/staircase/MTAMovingTimeFilter.cxx
        ../../../src/staircase/ProximitySensor.cxx
        ../../../src/staircase/StaircaseLooper.cxx
    INCLUDE_DIRS
        ../../../include
)

target_compile_definitions(${COMPONENT_LIB}
    PUBLIC LIGHTS_NUM=${CONFIG_LIGHTS_NUM}
    PUBLIC DEFAULT_ON_PERIOD=${CONFIG_DEFAULT_ON_PERIOD}
    PUBLIC DEBOUNCE_PERIOD=${CONFIG_DEBOUNCE_PERIOD}
    PUBLIC MAX_MOVINGS=${CONFIG_MAX_MOVINGS}
    PUBLIC MOVING_FINISH_DELTA=${CONFIG_MOVING_FINISH_DELTA}
    PUBLIC INITIAL_MOVING_DURATION=${CONFIG_INITIAL_MOVING_DURATION}
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Werror -fno-exceptions)