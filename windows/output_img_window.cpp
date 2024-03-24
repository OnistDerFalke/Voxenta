#include "output_img_window.h"

output_img_window::output_img_window(ImVec2 mws) {
    this->mws = mws;
}

void output_img_window::show() {

    //window resizing
    auto border = std::min(mws.x, mws.y) * 0.01f;
    ImGui::SetNextWindowPos(ImVec2((mws.x + border)/2,border));
    ImGui::SetNextWindowSize(ImVec2((mws.x-3*border)/2,(mws.y-(3*border))*2/3));
    ImGui::Begin("Output image", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::End();
}

void output_img_window::set_mws(ImVec2 size) {
    this->mws = size;
}