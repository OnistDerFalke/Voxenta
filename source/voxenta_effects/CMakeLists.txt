file(GLOB_RECURSE EFFECT_FILES "src/effects/*.cpp")

add_library(voxenta_effects SHARED
        src/effect.cpp
        ${EFFECT_FILES}
)

target_include_directories(voxenta_effects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(voxenta_effects PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(voxenta_effects PRIVATE
        ${OpenCV_LIBS}
        ImGui
)
