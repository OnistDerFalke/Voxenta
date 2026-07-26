#include "voxenta/windows/about_window.h"

#include <imgui.h>

about_window::about_window() = default;

void about_window::show(bool* open, ImVec2 mws) {
    if (*open) {
        ImGui::OpenPopup("About Voxenta");
        *open = false;
    }

    const ImVec2 about_size(mws.x * 2.0f / 6.0f, mws.y / 4.0f * 2.5f);
    ImGui::SetNextWindowPos(ImVec2(mws.x / 2 - about_size.x / 2, mws.y / 2 - about_size.y / 2));
    ImGui::SetNextWindowSize(about_size);

    bool window_open = true;
    if (ImGui::BeginPopupModal("About Voxenta", &window_open,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::BeginChild("about", ImVec2(0, 0), true);
        ImGui::TextWrapped("%s", "Welcome to Voxenta!\n\n"
            "Voxenta is a basic image processing UI tool made with ImGui, "
            "OpenGL and OpenCV with simple interface and effects descriptions. "
            "It was created to make things easier with image processing to use ready-made "
            "UI environment and focus only on the effects creation in C++ using OpenCV (default) "
            "or other libraries. Application was tested on Linux but it should also work after "
            "building it for Microsoft Windows environment.\n\n"
            "© 2024 OnistEngine.\n\n"
            "This application is provided under the terms of the "
            "MIT Licence, which means you have the right to copy, "
            "modify, and adapt our source code to your needs "
            "without obtaining my permission. However, please "
            "ensure to retain appropriate copyright notices and "
            "comply with the terms of the license.\n\n"
            "Thank you for using Voxenta!");
        ImGui::EndChild();
        ImGui::EndPopup();
    }
}