cmake_minimum_required(VERSION "2.8.0")
project("UM232H tests")

##

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules)

## executable targets

add_executable(gpio_test gpio_test.c um232h.c)
add_executable(nrf24_test nrf24_test.c um232h.c delay.c)

## find headers

FIND_PATH(FTDI_INCLUDE_DIR
NAMES ftdi.h
PATHS /usr/include
)

FIND_PATH(NRF24_INCLUDE_DIR
NAMES RF24.h nRF24L01.h
PATHS /usr/local/include
)

## find libraries

FIND_LIBRARY(FTDI_LIBRARY
NAMES ftdi
PATHS /usr/lib /usr/lib/x86_64-linux-gnu
)

FIND_LIBRARY(NRF24_LIBRARY
NAMES nrf24
PATHS /usr/local/lib
)

## set headers paths

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${NRF24_INCLUDE_DIR})
include_directories(${FTDI_INCLUDE_DIR})

## set libraries paths

target_link_libraries(nrf24_test ${FTDI_LIBRARY} ${NRF24_LIBRARY})
target_link_libraries(gpio_test ${FTDI_LIBRARY})
