project(shelter-utils)

add_library(${PROJECT_NAME} STATIC "")

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    src/buffer_reader.cpp
    src/buffer_writer.cpp
    src/ssid_device_descriptor.cpp
    src/base64.cpp
    src/crc8.cpp
)

target_link_libraries(${PROJECT_NAME} shelter-model exl)