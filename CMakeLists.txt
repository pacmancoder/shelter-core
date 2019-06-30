cmake_minimum_required(VERSION 3.10)

project(shelter-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libs)

add_subdirectory(shelter-model)
add_subdirectory(shelter-utils)

if (SHELTER_CORE_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif ()