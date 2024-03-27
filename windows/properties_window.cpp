#include "properties_window.h"
#include "imgui/imgui_internal.h"
#include "processing/image_processor.h"
#include "processing/processing_ui.h"
#include "about_window.h"

properties_window::properties_window(ImVec2 mws)
{
    this->mws = mws;
    this->data = processing_data();
    this->proc_ui = processing_ui();
    this->processor = image_processor();

    //Available features
    this->features[0] = const_cast<char*>("Brightness");
    this->features[1] = const_cast<char*>("Contrast");
    this->features[2] = const_cast<char*>("Negative");
    this->features[3] = const_cast<char*>("Gaussian Blur");
}

void properties_window::show() {

    just_uploaded = false;
    just_updated = false;

    //Setting new position and size
    auto border = std::min(mws.x, mws.y) * 0.01f;
    auto input_window_pos = ImVec2(border, mws.y*2/3);
    auto input_window_size = ImVec2(mws.x-2*border, (mws.y-3*border)*1/3);
    ImGui::SetNextWindowPos(input_window_pos);
    ImGui::SetNextWindowSize(input_window_size);

    //Context
    ImGui::Begin(" Properties", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_MenuBar);

    //Main menu
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load", "Ctrl+O")) {
            m_file_dialog_open = true;
            m_file_dialog_info.type = ImGuiFileDialogType_OpenFile;
            m_file_dialog_info.title = "Load image";
            m_file_dialog_info.fileName = "";
            m_file_dialog_info.directoryPath = std::filesystem::current_path();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            if(!modified_image.empty())
            {
                m_file_dialog_open = true;
                m_file_dialog_info.type = ImGuiFileDialogType_SaveFile;
                m_file_dialog_info.title = "Save image";
                m_file_dialog_info.fileName = "";
                m_file_dialog_info.directoryPath = std::filesystem::current_path();
            }
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About")) {
        if (ImGui::MenuItem("About Program")) {
            m_about_dialog_open = true;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();

    about.show(&m_about_dialog_open, mws);

    if(!base_image.empty()) {
        //Effect combo
        ImGui::BeginChild("effect_choice",
                          ImVec2((input_window_size.x) / 2.0f - 5.0f / 4.0f * ImGui::GetStyle().ItemSpacing.x, 20));
        ImGui::Combo("##combo", &selected_item, features, IM_ARRAYSIZE(features));
        ImGui::EndChild();
    }

    ImGui::Dummy(ImVec2(0, 2));

    //Properties of effect
    ImGui::BeginChild("properties",
                      ImVec2((input_window_size.x)/2.0f-5.0f/4.0f * ImGui::GetStyle().ItemSpacing.x,
                                             input_window_size.y-80), true);

    ImGui::Text("Effect properties:");
    ImGui::Dummy(ImVec2(0, 5));

    if(!base_image.empty())
        data = proc_ui.run_method(selected_item);

    if(ImGui::FileDialog(&m_file_dialog_open, &m_file_dialog_info, mws)) {
        file_path = m_file_dialog_info.resultPath;

        //Saving file from obtained image
        if(m_file_dialog_info.type == ImGuiFileDialogType_SaveFile)
        {
            if (!file_path.empty() && (!modified_image.empty())) {
               cv::imwrite(file_path.c_str(), modified_image);
            }
        }

        //Generating image from chosen file
        if(m_file_dialog_info.type == ImGuiFileDialogType_OpenFile) {
            just_uploaded = true;
            if (!file_path.empty()) {
                base_image = cv::imread(file_path.c_str());
            }
        }
    }

    if(!base_image.empty())
        modified_image = processor.process_image(base_image, selected_item, data, just_uploaded);
    just_updated = processor.did_update();

    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::BeginChild("description",
                      ImVec2((input_window_size.x)/2.0f - 3.5f/2.0f * ImGui::GetStyle().ItemSpacing.x,
                             input_window_size.y-80), true);

    ImGui::Text("Description:");
    ImGui::Dummy(ImVec2(0, 5));
    if(base_image.empty())
        ImGui::Text("%s", "Load the image first to apply any effects.");
    else ImGui::Text("%s", data.description);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
}

void properties_window::set_mws(ImVec2 size) {
    this->mws = size;
}

cv::Mat properties_window::get_base_image() {
    return base_image;
}

cv::Mat properties_window::get_modified_image() {
    return modified_image;
}
