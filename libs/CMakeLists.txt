project(libs)

if (SHELTER_CORE_ENABLE_TESTING)
    set(CATCH_BUILD_TESTING OFF CACHE BOOL "Build Catch2 SelfTest project")
    add_subdirectory(catch2)
endif()