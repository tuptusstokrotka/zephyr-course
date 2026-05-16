if(CONFIG_OUR_DRIVER)
    zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    target_sources(app PRIVATE our_driver.c)
endif()