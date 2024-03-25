#include "properties_window.h"

properties_window::properties_window(ImVec2 mws)
{
    this->mws = mws;
}

void properties_window::show() {

    just_uploaded = false;

    //Setting new position and size
    auto border = std::min(mws.x, mws.y) * 0.01f;
    ImGui::SetNextWindowPos(ImVec2(border, mws.y*2/3));
    ImGui::SetNextWindowSize(ImVec2(mws.x-2*border, (mws.y-3*border)*1/3));

    //Setting context
    ImGui::Begin("Properties", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    if (ImGui::Button("Load image")) {
        m_file_dialog_open = true;
        m_file_dialog_info.type = ImGuiFileDialogType_OpenFile;
        m_file_dialog_info.title = "Load image";
        m_file_dialog_info.fileName = "";
        m_file_dialog_info.directoryPath = std::filesystem::current_path();
    }
    if(ImGui::FileDialog(&m_file_dialog_open, &m_file_dialog_info, mws)) {
        file_path = m_file_dialog_info.resultPath;
        just_uploaded = true;

        //Generating image from chosen file
        if(!file_path.empty()) {
            base_image = cv::imread(file_path.c_str());
        }
    }
    ImGui::End();
}

void properties_window::set_mws(ImVec2 size) {
    this->mws = size;
}

cv::Mat properties_window::get_base_image() {
    return base_image;
}