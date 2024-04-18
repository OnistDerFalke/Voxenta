project(ImGui)

include(FetchContent)

CPMAddPackage(
        NAME imgui
        URL https://github.com/ocornut/imgui/archive/4f9ba19e520bea478f5cb654d37ef45e6404bd52.tar.gz
        URL_HASH SHA256=b165ecafcb917a9fd182cba81a67c511727cd71c041b1e05e26851e04729e720
        DOWNLOAD_ONLY TRUE
)

set(ImGui_SOURCE_FILES
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(ImGui STATIC ${ImGui_SOURCE_FILES})

target_include_directories(ImGui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

CPMAddPackage(
        NAME imnodes
        URL https://github.com/Nelarius/imnodes/archive/8563e1655bd9bb1f249e6552cc6274d506ee788b.tar.gz
        URL_HASH SHA256=e4342e6637e3a57c66e1fb42033addcd44f46662fd77189d2800696712b2b674
        OPTIONS
            "IMNODES_STANDALONE_PROJECT OFF"
            "IMNODES_IMGUI_TARGET_NAME ImGui"
)

target_link_libraries(ImGui PRIVATE glfw)
