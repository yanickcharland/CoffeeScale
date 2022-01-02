cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(CoffeeScale_Project)

pico_sdk_init()

file(GLOB FILES *.c *.h)

add_executable(CoffeeScale main.c hx711.c)
target_link_libraries(CoffeeScale pico_stdlib)
pico_add_extra_outputs(CoffeeScale)

pico_enable_stdio_usb(CoffeeScale 1)
pico_enable_stdio_uart(CoffeeScale 1)
