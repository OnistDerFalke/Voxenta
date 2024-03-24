#include "input_img_window.h"

input_img_window::input_img_window(ImVec2 mws) {
    this->mws = mws;
}

void input_img_window::show() {
    auto border = std::min(mws.x, mws.y) * 0.01f;
    ImGui::SetNextWindowPos(ImVec2(border,border));
    ImGui::SetNextWindowSize(ImVec2((mws.x-3*border)/2,(mws.y-(3*border))*2/3));
    ImGui::Begin("Input image", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::End();
}

void input_img_window::set_mws(ImVec2 size) {
    this->mws = size;
}

