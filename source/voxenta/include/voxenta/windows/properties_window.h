#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include "voxenta/windows/about_window.h"
#include "voxenta/effects/effect.h"
#include "voxenta/node_editor/node_editor.h"

#include <filesystem>

#include <imgui.h>
#include <imgui_node_editor.h>

#include <opencv2/opencv.hpp>

#include <stack>

class properties_window {
public:
    properties_window(ImVec2 mws, ax::NodeEditor::EditorContext* m_context);
    void show();

    void set_mws(ImVec2 size);
    cv::Mat get_base_image();
    cv::Mat get_modified_image();

    bool just_uploaded{};
    bool just_updated{};
private:
    enum Shortcuts {
        LOAD,
        SAVE,
#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
        RELOAD_EFFECTS,
#endif
        APPLY_EFFECT,
        UNDO_EFFECT,
        NUM_SHORTCUTS
    };
    ImVec2 mws;
    ax::NodeEditor::EditorContext* m_context;
    bool m_about_dialog_open{};
    about_window about;
    node_editor editor{};
    std::filesystem::path last_load_path;
    std::filesystem::path last_save_path;
    cv::Mat base_image;
    cv::Mat modified_image;
    std::stack<cv::Mat> history;
    size_t current_effect_idx;
    std::vector<bool> shortcut_active;
    std::vector<void (properties_window::*) ()> shortcut_methods;
    std::vector<ImGuiKey> shortcut_keys;

    void file_load();
    void file_save();
    void reload_effects();
    void apply_effect();
    void undo_effect();

    void handle_shortcuts();
    void set_shortcuts();
    bool shortcut_event(Shortcuts shortcut);

    void show_menu_bar();
    void show_node_editor(ImVec2 size);
    void show_node_explorer(ImVec2 size);
};

#endif
