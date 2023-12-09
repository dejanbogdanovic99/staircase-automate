idf_component_register(
    SRCS
        ../../../src/staircase/BasicLight.cxx
        ../../../src/staircase/Moving.cxx
        ../../../src/staircase/ProximitySensor.cxx
        ../../../src/staircase/StaircaseLooper.cxx
    INCLUDE_DIRS
        ../../../include
)

idf_component_set_property(
    staircase-automate
        COMPILE_OPTIONS "-Wall -Werror -fno-exceptions" APPEND
)