#include "properties_window.h"

properties_window::properties_window(ImVec2 mws)
{
    this->mws = mws;
}

void properties_window::show() {
    auto border = std::min(mws.x, mws.y) * 0.01f;
    ImGui::SetNextWindowPos(ImVec2(border, mws.y*2/3));
    ImGui::SetNextWindowSize(ImVec2(mws.x-2*border, (mws.y-3*border)*1/3));
    ImGui::Begin("Properties", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button("Load image")) {
        m_fileDialogOpen = true;
        m_fileDialogInfo.type = ImGuiFileDialogType_OpenFile;
        m_fileDialogInfo.title = "Load image";
        m_fileDialogInfo.fileName = "";
        m_fileDialogInfo.directoryPath = std::filesystem::current_path();
    }

    if(ImGui::FileDialog(&m_fileDialogOpen, &m_fileDialogInfo, mws))
    {
        filePath = m_fileDialogInfo.resultPath;
    }

    if(!filePath.empty())
        load_image();

    ImGui::End();
}

bool properties_window::load_image() {
    cv::Mat image = cv::imread(filePath.c_str());
   return false;
}

void properties_window::set_mws(ImVec2 size) {
    this->mws = size;
}
