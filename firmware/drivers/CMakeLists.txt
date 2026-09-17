add_library(drivers STATIC mpu6500.cpp)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(drivers PUBLIC
    hal
)