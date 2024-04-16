#include "voxenta/windows/properties_window.h"
#include "voxenta/windows/about_window.h"

#include "voxenta/effect_manager.h"

#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
#include <dlfcn.h>
#endif

#include <nfd.hpp>

properties_window::properties_window(ImVec2 mws, ax::NodeEditor::EditorContext* m_context)
{
    this->mws = mws;

    this->m_context = m_context;
    this->editor = node_editor(this->m_context);

    this->current_effect_idx = 0;

    this->shortcut_active.resize(Shortcuts::NUM_SHORTCUTS);
    this->last_load_path = std::filesystem::current_path();
    this->last_save_path = std::filesystem::current_path();

    set_shortcuts();
}

/* Shows the properties window and it's context */
void properties_window::show() {
    just_uploaded = false; //Image was loaded event
    just_updated = false; //Image changed (effect changed or was modified) event

    //Setting new position and size
    auto border = mws.x * 0.005f;
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

    //Sizes for context children
    auto node_editor_size = ImVec2(0.75f*input_window_size.x, 0.0f);
    auto node_explorer_size = ImVec2(input_window_size.x - 3.0f*input_window_pos.x - 0.75f*input_window_size.x, 0);

    //Show window context
    show_menu_bar();
    show_node_editor(node_editor_size);
    ImGui::SameLine(input_window_pos.x + node_editor_size.x, input_window_pos.x);
    show_node_explorer(node_explorer_size);

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
void properties_window::reload_effects() {
    current_effect_idx = std::clamp(this->current_effect_idx,
                                    static_cast<size_t>(0),
                                    effect_manager::effects().size() - 1);
    this->just_updated = true;
}

/* Applies effect to the image */
void properties_window::apply_effect() {
    history.push(base_image.clone());
    base_image = modified_image.clone();
    just_updated = true;
    just_uploaded = true;
}

/* Undo latest effect of the image */
void properties_window::undo_effect() {
    if(!history.empty()) {
        base_image = history.top().clone();
        history.pop();
        just_updated = true;
        just_uploaded = true;
    }
}

/* Handles loading the image */
void properties_window::file_load() {
    NFD::UniquePathU8 outPath;

    //No need to differentiate between image formats
    constexpr nfdfilteritem_t filters[1] = {
            { "Images", "jpg,jpeg,jfif,png" }
    };

    nfdresult_t result = NFD::OpenDialog(
            outPath,
            filters,
            IM_ARRAYSIZE(filters),
            last_load_path.parent_path().generic_string().c_str());

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
        NFD::UniquePathU8 outPath;

        //Prefer lossless image formats
        constexpr nfdfilteritem_t filters[2] = {
                { "PNG Image", "png" },
                { "JPEG Image", "jpg,jpeg,jfif" }
        };

        nfdresult_t result = NFD::SaveDialog(
                outPath,
                filters,
                IM_ARRAYSIZE(filters),
                last_save_path.parent_path().generic_string().c_str(),
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



/* Checks if event was started */
bool properties_window::shortcut_event(properties_window::Shortcuts shortcut) {
    if(!ImGui::GetIO().KeyCtrl) {
        return false;
    }
    if(!ImGui::IsKeyPressed(shortcut_keys[shortcut]))
        shortcut_active[shortcut] = false;
    return ImGui::IsKeyPressed(shortcut_keys[shortcut]);
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

/* Sets shortcuts methods pointer and keys for all shortcuts */
void properties_window::set_shortcuts() {
    this->shortcut_methods.resize(Shortcuts::NUM_SHORTCUTS);
    this->shortcut_keys.resize(Shortcuts::NUM_SHORTCUTS);

    this->shortcut_methods[Shortcuts::LOAD] = &properties_window::file_load;
    this->shortcut_methods[Shortcuts::SAVE] = &properties_window::file_save;
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    this->shortcut_methods[Shortcuts::RELOAD_EFFECTS] = &properties_window::reload_effects;
#endif
    this->shortcut_methods[Shortcuts::APPLY_EFFECT] = &properties_window::apply_effect;
    this->shortcut_methods[Shortcuts::UNDO_EFFECT] = &properties_window::undo_effect;

    this->shortcut_keys[Shortcuts::LOAD] =  ImGui::GetKeyIndex(ImGuiKey_O);
    this->shortcut_keys[Shortcuts::SAVE] = ImGui::GetKeyIndex(ImGuiKey_S);
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    this->shortcut_keys[Shortcuts::RELOAD_EFFECTS] =  ImGui::GetKeyIndex(ImGuiKey_R);
#endif
    this->shortcut_keys[Shortcuts::APPLY_EFFECT] = ImGui::GetKeyIndex(ImGuiKey_A);
    this->shortcut_keys[Shortcuts::UNDO_EFFECT] =  ImGui::GetKeyIndex(ImGuiKey_Z);
}



/* Shows menu bar on the top of properties context */
void properties_window::show_menu_bar() {
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
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
        if (ImGui::MenuItem("Reload effects", "Ctrl+R"))
            reload_effects();
#endif
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
}

/* Shows node editor as a children of properties context */
void properties_window::show_node_editor(ImVec2 size) {
    editor.show(size);
}

/* Shows node explorer as a children of properties context */
void properties_window::show_node_explorer(ImVec2 size) {
    const effect_list_t& effects = effect_manager::effects();

    ImGui::BeginChild("properties", size, true);
    if (ImGui::BeginCombo("##effect_combo", effects[current_effect_idx].get().get_name())) {
        for (int n = 0; n < effects.size(); n++) {
            const bool is_selected = (current_effect_idx == n);
            if (ImGui::Selectable(effects[n].get().get_name(), is_selected)) {
                just_updated = true;
                current_effect_idx = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Text("Effect properties:");
    ImGui::Dummy(ImVec2(0, 5));

    just_updated = just_updated || effects[current_effect_idx].get().run_ui();
    if(!base_image.empty() && (just_updated || modified_image.empty())) {
        modified_image = effects[this->current_effect_idx].get().run(base_image);
    }

    ImGui::EndChild();
}
