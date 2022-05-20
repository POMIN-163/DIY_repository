cmake_minimum_required(VERSION 3.12)

#set(PICO_PLATFORM host)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(scoppy)

pico_sdk_init()

pico_add_subdirectory(scoppy-usb)
add_subdirectory(../scoppy/lib scoppy-lib)

message(STATUS "Scoppy cmake")

##########################################
# 
# You can set custom values for some cmake variables in scoppy-local.cmake
# That file is not tracked by git and so won't cause problems when you pull the
# latest version of scoppy from github.
#
# https://stackoverflow.com/questions/15201064/cmake-conditional-preprocessor-define-on-code
#

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scoppy-local.cmake")
    message(STATUS "Including scoppy-local.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/scoppy-local.cmake")

    if(SCOPPY_COMPILE_DEFS)
        message(STATUS "    SCOPPY_COMPILE_DEFS: ${SCOPPY_COMPILE_DEFS}")
        add_compile_definitions(${SCOPPY_COMPILE_DEFS})
    endif()

    message(STATUS "  end local definitions")

else()
    message(STATUS "scoppy-local.cmake not found. Using default definitions.")
endif()

#
# end customisation
#
##########################################

set(PICO_SCOPPY_VERSION "10")
set(SCOPPY_TARGET "scoppy-pico-v${PICO_SCOPPY_VERSION}")

# applies to this directory and below
#add_compile_options(-Wall -Werror -Wno-error=unused-variable -Werror=unused-but-set-variable -DPICO_SCOPPY_VERSION=${PICO_SCOPPY_VERSION})

if(DEFINED PICO_SCOPPY_BUILD_NUMBER)
    add_compile_definitions(PICO_SCOPPY_BUILD_NUMBER=${PICO_SCOPPY_BUILD_NUMBER})
endif()

add_compile_definitions(
    PICO_SCOPPY_VERSION=${PICO_SCOPPY_VERSION}
)

add_executable(${SCOPPY_TARGET})

pico_generate_pio_header(${SCOPPY_TARGET} ${CMAKE_CURRENT_LIST_DIR}/pico-scoppy-triggering.pio)

target_sources(${SCOPPY_TARGET} PRIVATE
    pico-scoppy.c
    pico-scoppy.h
    pico-scoppy-core0-looper.c
    pico-scoppy-core0-looper.h
    pico-scoppy-ctx.c
    pico-scoppy-ctx.h
    pico-scoppy-cont-sampling.c
    pico-scoppy-cont-sampling.h
    pico-scoppy-non-cont-sampling.h
    pico-scoppy-non-cont-sampling.c
    pico-scoppy-pwm-sig-gen.c
    pico-scoppy-pwm-sig-gen.h
    pico-scoppy-samples.c
    pico-scoppy-samples.h
    pico-scoppy-util.c
    pico-scoppy-util.h
    pico-scoppy-voltage-range.c
    pico-scoppy-voltage-range.h
    scoppy-pio.c
    scoppy-pio.h
)

# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
# NB. This applies the options to the whole SDK

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:Debug>:PARAM_ASSERTIONS_ENABLE_ALL=1>
  )

target_link_libraries(${SCOPPY_TARGET} PRIVATE
    pico_bootsel_via_double_reset    
    pico_multicore
    pico_stdlib
    pico_unique_id
    hardware_adc
    hardware_clocks
    hardware_dma
    hardware_irq
    hardware_pio
    hardware_pwm
    scoppy_usb
    scoppy-libs
)

# Only set this for DEBUG builds!
#pico_set_binary_type(${SCOPPY_TARGET} no_flash)

# create map/bin/hex file etc.
pico_add_extra_outputs(${SCOPPY_TARGET})
