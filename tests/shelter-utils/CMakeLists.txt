project(shelter-utils-test)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ssid_device_descriptor.cpp
    base64.cpp
    crc8.cpp
)

target_link_libraries(${PROJECT_NAME} Catch2 shelter-utils)