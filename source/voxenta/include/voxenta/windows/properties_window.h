#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include "voxenta/windows/about_window.h"
#include "voxenta/effects/effect.h"

#include <filesystem>

#include <imgui.h>
#include <opencv4/opencv2/opencv.hpp>

class properties_window {
public:
    explicit properties_window(ImVec2 mws);
    void set_mws(ImVec2 size);
    cv::Mat get_base_image();
    cv::Mat get_modified_image();
    void show();
    bool just_uploaded{};
    bool just_updated{};
private:
    enum Shortcuts {
        LOAD,
        SAVE,
        RELOAD_EFFECTS,
        NUM_SHORTCUTS
    };
    ImVec2 mws;
    bool m_about_dialog_open{};
    about_window about;
    std::filesystem::path last_load_path;
    std::filesystem::path last_save_path;
    cv::Mat base_image;
    cv::Mat modified_image;
    void *effects_lib;
    std::vector<effect*>* effects;
    size_t current_effect_idx;
    void load_effects();
    bool shortcut_event(Shortcuts shortcut);
    void handle_fileload();
    void handle_filesave();
    void handle_shortcuts();
};

#endif
