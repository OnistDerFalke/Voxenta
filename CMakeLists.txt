cmake_minimum_required(VERSION 3.20)
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
        DEPENDS $<TARGET_FILE:ImGui> $<TARGET_FILE:voxenta_effects>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:ImGui> $<TARGET_FILE_DIR:voxenta>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:voxenta_effects> $<TARGET_FILE_DIR:voxenta>
)

add_dependencies(voxenta voxenta_copy_deps)

set_target_properties(voxenta
        PROPERTIES
        BUILD_WITH_INSTALL_RPATH true
        LINK_OPTIONS "-Wl,--disable-new-dtags"
)

install(TARGETS ImGui voxenta_effects voxenta DESTINATION bin)
