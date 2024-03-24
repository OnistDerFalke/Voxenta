cmake_minimum_required(VERSION 3.20)
project(voxenta)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)

set(APP_TARGET voxenta)

add_subdirectory(imgui)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(${APP_TARGET} main.cpp
        windows/main_window.cpp
        windows/main_window.h
        windows/input_img_window.cpp
        windows/input_img_window.h
        windows/properties_window.cpp
        windows/properties_window.h
        imgui/imgui_filedialog.cpp
        windows/output_img_window.cpp
        windows/output_img_window.h)
target_link_libraries(${APP_TARGET} imgui)

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(
        ${APP_TARGET}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${EXTRA_LIBS}
)