add_executable(voxenta
        src/main.cpp

        src/windows/about_window.cpp
        src/windows/input_img_window.cpp
        src/windows/main_window.cpp
        src/windows/output_img_window.cpp
        src/windows/properties_window.cpp
)

target_include_directories(voxenta PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        $<TARGET_PROPERTY:voxenta_effects,INTERFACE_INCLUDE_DIRECTORIES>

        ${OpenCV_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(voxenta PRIVATE
        ${OpenCV_LIBS}
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ImGui
        nfd
)

if (VOXENTA_EFFECTS_HOT_RELOAD)
    target_compile_definitions(voxenta PRIVATE VOXENTA_EFFECTS_HOT_RELOAD)
    set_property(TARGET voxenta PROPERTY ENABLE_EXPORTS ON)
else()
    target_link_libraries(voxenta PRIVATE $<LINK_LIBRARY:WHOLE_ARCHIVE,voxenta_effects>)
endif()
