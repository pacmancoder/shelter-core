project(shelter-utils-test)

add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ssid_device_descriptor_test.cpp
    base64_test.cpp
)

target_link_libraries(${PROJECT_NAME} Catch2 shelter-utils)