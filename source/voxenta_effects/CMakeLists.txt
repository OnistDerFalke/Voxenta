file(GLOB_RECURSE VOXENTA_EFFECTS_SOURCE_FILES "src/effects/*.cpp")
list(APPEND VOXENTA_EFFECTS_SOURCE_FILES "src/effect.cpp")

if (VOXENTA_EFFECTS_HOT_RELOAD)
    add_library(voxenta_effects SHARED ${VOXENTA_EFFECTS_SOURCE_FILES})
else()
    add_library(voxenta_effects STATIC ${VOXENTA_EFFECTS_SOURCE_FILES})
endif()

target_include_directories(voxenta_effects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(voxenta_effects PRIVATE
        $<TARGET_PROPERTY:ImGui,INTERFACE_INCLUDE_DIRECTORIES>
        ${OpenCV_INCLUDE_DIRS}
)

if (VOXENTA_EFFECTS_HOT_RELOAD)
    target_compile_definitions(voxenta_effects PRIVATE VOXENTA_EFFECTS_HOT_RELOAD)
endif()
