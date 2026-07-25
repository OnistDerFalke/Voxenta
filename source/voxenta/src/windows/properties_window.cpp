#include "voxenta/windows/properties_window.h"
#include "voxenta/windows/about_window.h"

#include "voxenta/effect_manager.h"

#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
#include <dlfcn.h>
#endif

#include <nfd.hpp>

properties_window::properties_window(ImVec2 mws)
{
    this->mws = mws;

    this->editor = node_editor();

    this->shortcut_active.resize(Shortcuts::NUM_SHORTCUTS);

    this->last_load_path = std::filesystem::current_path();
    this->last_save_path = std::filesystem::current_path();

    set_shortcuts();
}

/* Shows the properties window and it's context */
void properties_window::show() {
    just_uploaded = false; //Image was loaded event
    just_updated = false; //Image changed (effect changed or was modified) event

    editor.set_input_image(base_image);
    editor.set_input_extension(last_load_path.extension().string());

    //Node editor covers the entire application window - this is now the only view
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(mws);

    ImGui::Begin("Voxenta", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_MenuBar);

    handle_shortcuts();

    show_menu_bar();

    show_node_editor(ImGui::GetContentRegionAvail());

    if (pending_save_) {
        file_save();
        pending_save_ = false;
    }
    if (pending_apply_) {
        apply_effect();
        pending_apply_ = false;
    }

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
    this->just_updated = true;
}

void properties_window::request_save() { pending_save_ = true; }
void properties_window::request_apply() { pending_apply_ = true; }

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
    if (!modified_image.empty()) {
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
            !is_directory(last_load_path) ? (last_load_path.stem().string() + "_out" + last_load_path.extension().string()).c_str() : nullptr);

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
    if (!ImGui::GetIO().KeyCtrl) {
        return false;
    }
    if (!ImGui::IsKeyPressed(shortcut_keys[shortcut]))
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
    this->shortcut_methods[Shortcuts::SAVE] = &properties_window::request_save;
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    this->shortcut_methods[Shortcuts::RELOAD_EFFECTS] = &properties_window::reload_effects;
#endif
    this->shortcut_methods[Shortcuts::APPLY_EFFECT] = &properties_window::request_apply;
    this->shortcut_methods[Shortcuts::UNDO_EFFECT] = &properties_window::undo_effect;

    this->shortcut_keys[Shortcuts::LOAD] = ImGui::GetKeyIndex(ImGuiKey_O);
    this->shortcut_keys[Shortcuts::SAVE] = ImGui::GetKeyIndex(ImGuiKey_S);
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
    this->shortcut_keys[Shortcuts::RELOAD_EFFECTS] = ImGui::GetKeyIndex(ImGuiKey_R);
#endif
    this->shortcut_keys[Shortcuts::APPLY_EFFECT] = ImGui::GetKeyIndex(ImGuiKey_A);
    this->shortcut_keys[Shortcuts::UNDO_EFFECT] = ImGui::GetKeyIndex(ImGuiKey_Z);
}



/* Shows menu bar on the top of properties context */
void properties_window::show_menu_bar() {
    ImGui::BeginMenuBar();
    ImGui::SetWindowFontScale(0.8f);

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load", "Ctrl+O"))
            file_load();
        if (ImGui::MenuItem("Save", "Ctrl+S"))
            request_save();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Minimap"))
    {
        const char* names[] = { "Top Left", "Top Right", "Bottom Left", "Bottom Right" };
        int locations[] = { ImNodesMiniMapLocation_TopLeft,
                           ImNodesMiniMapLocation_TopRight,
                           ImNodesMiniMapLocation_BottomLeft,
                           ImNodesMiniMapLocation_BottomRight };

        for (int i = 0; i < 4; i++)
        {
            bool selected = editor.get_minimap_location() == locations[i];
            if (ImGui::MenuItem(names[i], nullptr, &selected))
                editor.set_minimap_location(locations[i]);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About")) {
        if (ImGui::MenuItem("About Program")) {
            m_about_dialog_open = true;
        }
        ImGui::EndMenu();
    }
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndMenuBar();

    about.show(&m_about_dialog_open, mws);
}

/* Shows node editor as a children of properties context */
void properties_window::show_node_editor(ImVec2 size) {
    ImGui::BeginChild("editor", size, true);
    editor.show();
    ImGui::EndChild();

    if (!base_image.empty()) {
        cv::Mat graph_output = editor.get_output();
        modified_image = graph_output.empty() ? base_image : graph_output;
        just_updated = true;
    }
}