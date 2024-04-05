#include "windows/properties_window.h"
#include "windows/about_window.h"

#include "processing/image_processor.h"
#include "processing/processing_ui.h"

#include <nfd.hpp>

properties_window::properties_window(ImVec2 mws)
{
    this->mws = mws;
    this->data = containers::processing_data();
    this->proc_ui = processing_ui();
    this->processor = image_processor();

    //Available features
    this->features[0] = const_cast<char*>("Brightness");
    this->features[1] = const_cast<char*>("Contrast");
    this->features[2] = const_cast<char*>("Negative");
    this->features[3] = const_cast<char*>("Grayscale");
    this->features[4] = const_cast<char*>("Binarization");

    this->last_load_path = std::filesystem::current_path();
    this->last_save_path = std::filesystem::current_path();
}

/* Shows the properties window and it's context */
void properties_window::show() {

    just_uploaded = false; //image was loaded event
    just_updated = false; //image changed (effect changed or was modified) event

    //Setting new position and size
    auto border = std::min(mws.x, mws.y) * 0.01f;
    auto input_window_pos = ImVec2(border, mws.y*2/3);
    auto input_window_size = ImVec2(mws.x-2*border, (mws.y-3*border)*1/3);
    ImGui::SetNextWindowPos(input_window_pos);
    ImGui::SetNextWindowSize(input_window_size);

    //Context
    ImGui::Begin(" Properties", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    //Handle shortcuts events
    handle_shortcuts();

    //Main menu, focused windows preparation
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load", "Ctrl+O"))
            handle_fileload();
        if (ImGui::MenuItem("Save", "Ctrl+S"))
            handle_filesave();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About")) {
        if (ImGui::MenuItem("About Program")) {
            m_about_dialog_open = true;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();

    //Shows about-window if it was opened in from menu
    about.show(&m_about_dialog_open, mws);

    //To choose the effect image needs to be loaded
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

    //Getting image effect properties data set in UI
    if(!base_image.empty())
        data = proc_ui.run_method(selected_item);

    //Applying the effects with retrieved properties data
    if(!base_image.empty())
        modified_image = processor.process_image(base_image, selected_item, data, just_uploaded);

    just_updated = processor.did_update();

    ImGui::EndChild();
    ImGui::SameLine();

    //Showing description in right properties window child
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::BeginChild("description",
                      ImVec2((input_window_size.x)/2.0f - 3.5f/2.0f * ImGui::GetStyle().ItemSpacing.x,
                             input_window_size.y-80), true);

    ImGui::Text("Description:");
    ImGui::Dummy(ImVec2(0, 5));
    if(base_image.empty())
        ImGui::TextWrapped("%s", "Load the image first to apply any effects.");
    else ImGui::TextWrapped("%s", data.description);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
}

/* Updates the window size */
void properties_window::set_mws(ImVec2 size) {
    this->mws = size;
}

/* Returns the base image */
cv::Mat properties_window::get_base_image() {
    return base_image;
}

/* Returns modified image with effect applied */
cv::Mat properties_window::get_modified_image() {
    return modified_image;
}

/* Checks if event was started */
bool properties_window::shortcut_event(properties_window::Shortcuts shortcut) {
    if(!ImGui::GetIO().KeyCtrl) return false;
    bool shortcut_activated = false;
    switch (shortcut) {
        case LOAD:
            shortcut_activated = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O));
            break;
        case SAVE:
            shortcut_activated = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
            break;
        default:
            break;
    }
    return shortcut_activated;
}

/* Handles loading the image */
void properties_window::handle_fileload() {
    NFD::UniquePathN outPath;

    //No need to differentiate between image formats
    constexpr nfdfilteritem_t filters[1] = {
            { "Images", "jpg,jpeg,jfif,png" }
    };

    nfdresult_t result = NFD::OpenDialog(
            outPath,
            filters,
            IM_ARRAYSIZE(filters),
            last_load_path.parent_path().c_str());

    if (result == NFD_OKAY && outPath != nullptr) {
        just_uploaded = true;
        base_image = cv::imread(outPath.get());

        //Image loaded successfully
        if (base_image.data != nullptr) {
            last_load_path = outPath.get();
        }
        else {
            fprintf(stderr, "invalid file format\n");
        }
    }
    else if (result == NFD_ERROR) {
        fprintf(stderr, "nfd error: %s\n", NFD::GetError());
    }
}

/* Handles saving the image */
void properties_window::handle_filesave() {
    if(!modified_image.empty()) {
        NFD::UniquePathN outPath;

        //Prefer lossless image formats
        constexpr nfdfilteritem_t filters[2] = {
                { "PNG Image", "png" },
                { "JPEG Image", "jpg,jpeg,jfif" }
        };

        nfdresult_t result = NFD::SaveDialog(
                outPath,
                filters,
                IM_ARRAYSIZE(filters),
                last_save_path.parent_path().c_str(),
                !is_directory(last_load_path) ?(last_load_path.stem().string() + "_out" + last_load_path.extension().string()).c_str(): nullptr);

        if (result == NFD_OKAY && outPath != nullptr) {
            cv::imwrite(outPath.get(), modified_image);
            last_save_path = outPath.get();
        }
        else if (result == NFD_ERROR) {
            fprintf(stderr, "nfd error: %s\n", NFD::GetError());
        }
    }
}

/* Handles keyboard hotkeys/shortcuts */
void properties_window::handle_shortcuts() {
    for (int i = 0; i < Shortcuts::NUM_SHORTCUTS; ++i) {
        switch (static_cast<Shortcuts>(i)) {
            case LOAD:
                if (shortcut_event(LOAD))
                    handle_fileload();
                break;
            case SAVE:
                if (shortcut_event(SAVE))
                    handle_filesave();
                break;
            default:
                break;
        }
    }
}
