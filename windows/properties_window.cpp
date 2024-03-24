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

    std::filesystem::path filePath;
    if(ImGui::FileDialog(&m_fileDialogOpen, &m_fileDialogInfo, mws))
    {
        filePath = m_fileDialogInfo.resultPath;
    }

    ImGui::End();
}

bool properties_window::load_image() {
//    int image_width = 0;
//    int image_height = 0;
//    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
//    if (image_data == NULL)
//        return false;
    return false;
}

void properties_window::set_mws(ImVec2 size) {
    this->mws = size;
}
