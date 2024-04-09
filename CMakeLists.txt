cmake_minimum_required(VERSION 3.20)
project(voxenta)

set(CMAKE_CXX_STANDARD 23)

set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)

set(APP_TARGET voxenta)

find_package(OpenCV REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(external/imgui)
add_subdirectory(external/nativefiledialog-extended)

file(GLOB_RECURSE EFFECT_FILES "src/effects/*.cpp")

add_executable(${APP_TARGET}
        src/main.cpp

        src/windows/about_window.cpp
        src/windows/input_img_window.cpp
        src/windows/main_window.cpp
        src/windows/output_img_window.cpp
        src/windows/properties_window.cpp

        ${EFFECT_FILES}
)

target_include_directories(${APP_TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include

        ${OpenCV_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(${APP_TARGET} PRIVATE
        ${OpenCV_LIBS}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ImGui
        nfd
)
