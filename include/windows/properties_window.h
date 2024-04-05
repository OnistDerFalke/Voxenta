#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include "processing/processing_ui.h"
#include "processing/image_processor.h"
#include "about_window.h"

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
    ImVec2 mws;
    bool m_about_dialog_open{};
    about_window about;
    std::filesystem::path last_load_path;
    std::filesystem::path last_save_path;
    cv::Mat base_image;
    cv::Mat modified_image;
    processing_ui proc_ui;
    image_processor processor;
    containers::processing_data data;
    const char* features[5]{};
    int selected_item = 0;
};

#endif
