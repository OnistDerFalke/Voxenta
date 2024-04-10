#include "voxenta/windows/properties_window.h"
#include "voxenta/windows/about_window.h"

#include "voxenta/effects/effect.h"

#include <dlfcn.h>
#include <nfd.hpp>

properties_window::properties_window(ImVec2 mws)
{
    this->mws = mws;

    this->effects_lib = nullptr;
    this->effects = nullptr;
    this->current_effect_idx = 0;
    this->load_effects();
    this->shortcut_active.resize(Shortcuts::NUM_SHORTCUTS);
    this->last_load_path = std::filesystem::current_path();
    this->last_save_path = std::filesystem::current_path();

    set_shortcuts();
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
            file_load();
        if (ImGui::MenuItem("Save", "Ctrl+S"))
            file_save();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Effects")) {
        if (ImGui::MenuItem("Apply", "Ctrl+A"))
            apply_effect();
        if (ImGui::MenuItem("Undo", "Ctrl+Z"))
            undo_effect();
        if (ImGui::MenuItem("Reload effects", "Ctrl+R"))
            load_effects();
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

    //Effect combo
    ImGui::BeginChild("effect_choice",
                      ImVec2((input_window_size.x) / 2.0f - 5.0f / 4.0f * ImGui::GetStyle().ItemSpacing.x, 20));
    if (ImGui::BeginCombo("##effect_combo", effects->at(current_effect_idx)->get_name())) {
        for (int n = 0; n < effects->size(); n++) {
            const bool is_selected = (current_effect_idx == n);
            if (ImGui::Selectable(effects->at(n)->get_name(), is_selected)) {
                just_updated = true;
                current_effect_idx = n;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::EndChild();
    ImGui::Dummy(ImVec2(0, 2));

    //Properties of effect
    ImGui::BeginChild("properties",
                      ImVec2((input_window_size.x)/2.0f-5.0f/4.0f * ImGui::GetStyle().ItemSpacing.x,
                                             input_window_size.y-80), true);
    ImGui::Text("Effect properties:");
    ImGui::Dummy(ImVec2(0, 5));

    //Getting image effect properties data set in UI
    just_updated = just_updated || effects->at(current_effect_idx)->run_ui();
    if(!base_image.empty() && (just_updated || modified_image.empty())) {
        modified_image = effects->at(this->current_effect_idx)->run(base_image);
    }

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
    else
        ImGui::TextWrapped("%s", effects->at(this->current_effect_idx)->get_description());
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

/* Loads available effects*/
void properties_window::load_effects() {
    if (this->effects_lib != nullptr) {
        dlclose(this->effects_lib);
    }

    this->effects_lib = dlopen("./libvoxenta_effects.so", RTLD_LAZY | RTLD_DEEPBIND);
    this->effects = static_cast<std::vector<effect*>*>(dlsym(effects_lib, "g_effects"));
    this->current_effect_idx = std::clamp(this->current_effect_idx, 0ul, this->effects->size() - 1);

    this->just_updated = true;
}

void properties_window::apply_effect() {
    history.push(base_image.clone());
    base_image = modified_image.clone();
    just_updated = true;
    just_uploaded = true;
}

void properties_window::undo_effect() {
    if(!history.empty()) {
        base_image = history.top().clone();
        history.pop();
        just_updated = true;
        just_uploaded = true;
    }
}

/* Checks if event was started */
bool properties_window::shortcut_event(properties_window::Shortcuts shortcut) {
    if(!ImGui::GetIO().KeyCtrl) {
        return false;
    }
    if(!ImGui::IsKeyPressed(shortcut_keys[shortcut]))
        shortcut_active[shortcut] = false;
    return ImGui::IsKeyPressed(shortcut_keys[shortcut]);
}

/* Handles loading the image */
void properties_window::file_load() {
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
        just_updated = true;
        std::stack<cv::Mat>().swap(history);
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
void properties_window::file_save() {
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
        if (shortcut_event(static_cast<Shortcuts>(i)) && !shortcut_active[i]) {
            shortcut_active[i] = true;
            (this->*shortcut_methods[i])();
        }
    }
}

void properties_window::set_shortcuts() {
    this->shortcut_methods.resize(Shortcuts::NUM_SHORTCUTS);
    this->shortcut_keys.resize(Shortcuts::NUM_SHORTCUTS);

    this->shortcut_methods[Shortcuts::LOAD] = &properties_window::file_load;
    this->shortcut_methods[Shortcuts::SAVE] = &properties_window::file_save;
    this->shortcut_methods[Shortcuts::RELOAD_EFFECTS] = &properties_window::load_effects;
    this->shortcut_methods[Shortcuts::APPLY_EFFECT] = &properties_window::apply_effect;
    this->shortcut_methods[Shortcuts::UNDO_EFFECT] = &properties_window::undo_effect;

    this->shortcut_keys[Shortcuts::LOAD] =  ImGui::GetKeyIndex(ImGuiKey_O);
    this->shortcut_keys[Shortcuts::SAVE] = ImGui::GetKeyIndex(ImGuiKey_S);
    this->shortcut_keys[Shortcuts::RELOAD_EFFECTS] =  ImGui::GetKeyIndex(ImGuiKey_R);
    this->shortcut_keys[Shortcuts::APPLY_EFFECT] = ImGui::GetKeyIndex(ImGuiKey_A);
    this->shortcut_keys[Shortcuts::UNDO_EFFECT] =  ImGui::GetKeyIndex(ImGuiKey_Z);
}
