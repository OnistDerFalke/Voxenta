cmake_minimum_required(VERSION 3.20)

set(CPM_DOWNLOAD_VERSION 0.39.0)
set(CPM_HASH_SUM "66639bcac9dd2907b2918de466783554c1334446b9874e90d38e3778d404c2ef")

if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION} EXPECTED_HASH SHA256=${CPM_HASH_SUM}
)

include(${CPM_DOWNLOAD_LOCATION})

project(voxenta)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INSTALL_RPATH "${LIB_INSTALL_PATH}:$ORIGIN")

find_package(OpenCV REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(external/imgui)
add_subdirectory(external/nativefiledialog-extended)

add_subdirectory(source/voxenta_effects)
add_subdirectory(source/voxenta)

add_custom_target(voxenta_copy_deps
        DEPENDS $<TARGET_FILE:voxenta_effects>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:voxenta_effects> $<TARGET_FILE_DIR:voxenta>
)

add_dependencies(voxenta voxenta_copy_deps)

set_target_properties(voxenta
        PROPERTIES
        BUILD_WITH_INSTALL_RPATH true
        LINK_OPTIONS "-Wl,--disable-new-dtags"
)

install(TARGETS voxenta_effects voxenta DESTINATION bin)
