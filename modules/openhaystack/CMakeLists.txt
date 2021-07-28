if(CONFIG_OPENHAYSTACK)
zephyr_interface_library_named(openhaystack)

zephyr_include_directories(src)

#zephyr_library()
zephyr_library_sources(
    src/openhaystack.c
)

#zephyr_library_link_libraries(OPENHAYSTACK)
#target_link_libraries(OPENHAYSTACK INTERFACE zephyr_interface)
endif()
