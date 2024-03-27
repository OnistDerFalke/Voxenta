#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include <opencv4/opencv2/opencv.hpp>
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_filedialog.h"
#include "processing/processing_ui.h"
#include "processing/image_processor.h"
#include "about_window.h"

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
    bool m_file_dialog_open{};
    bool m_about_dialog_open{};
    ImFileDialogInfo m_file_dialog_info;
    about_window about;
    std::filesystem::path file_path;
    cv::Mat base_image;
    cv::Mat modified_image;
    processing_ui proc_ui;
    image_processor processor;
    processing_data data;
    const char* features[4]{};
    int selected_item = 0;
};


#endif
