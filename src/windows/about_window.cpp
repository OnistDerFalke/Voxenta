#include "windows/about_window.h"

#include <imgui.h>

about_window::about_window() = default;

/* Shows the about-window */
void about_window::show(bool* open, ImVec2 mws) {
    if(!*open) return;

    //Setting size, position and focus
    ImGui::SetNextWindowPos(ImVec2(mws.x/2 - mws.x/6, mws.y/2-mws.y/4));
    ImGui::SetNextWindowSize(ImVec2(mws.x*2/6, mws.y/4));
    ImGui::SetNextWindowFocus();

    //Setting about-window context
    if(ImGui::Begin("About Voxenta", open,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
        ImGui::BeginChild("about", ImVec2(0,0), true);
        ImGui::TextWrapped("%s", "Welcome to Voxenta!\n\n"
                    "Voxenta is a basic image processing UI tool made with ImGui, "
                    "OpenGL and OpenCV with simple interface and effects descriptions. "
                    "It was created to make things easier with image processing to use ready-made "
                    "UI environment and focus only on the effects creation in C++ using OpenCV (default) "
                    "or other libraries. Application was tested on Linux but it should also work after "
                    "building it for Microsoft Windows environment.\n\n"
                    "© 2024 OnistEngine.\n\n"
                    "This application is provided under the terms of the\n"
                    "MIT Licence, which means you have the right to copy,\n"
                    "modify, and adapt our source code to your needs \n"
                    "without obtaining my permission. However, please \n"
                    "ensure to retain appropriate copyright notices and\n"
                    "comply with the terms of the license.\n\n"
                    "Thank you for using Voxenta!");
        ImGui::EndChild();
        ImGui::End();
    }
}


