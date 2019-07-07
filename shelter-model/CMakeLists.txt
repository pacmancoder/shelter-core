project(shelter-model)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    src/shelter-model/device_descriptor.cpp
    src/shelter-model/error.cpp
)