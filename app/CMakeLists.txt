cmake_minimum_required(VERSION 3.13.1)

# Find OpenHaystack module
list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/../modules/openhaystack)

# Find Zephyr. This also loads Zephyr's build system.
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(openhaystack-firmware)

# Source files to the "app" target
target_sources(app PRIVATE src/main.c)
if(CONFIG_USB_UART_CONSOLE)
  target_sources(app PRIVATE src/debug.c)
endif()
