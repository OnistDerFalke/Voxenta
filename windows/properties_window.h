#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include <opencv4/opencv2/opencv.hpp>
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_filedialog.h"

class properties_window {
public:
    explicit properties_window(ImVec2 mws);
    void set_mws(ImVec2 size);
    cv::Mat get_base_image();
    void show();
    bool just_uploaded;
private:
    ImVec2 mws;
    bool m_file_dialog_open;
    ImFileDialogInfo m_file_dialog_info;
    std::filesystem::path file_path;
    cv::Mat base_image;
    cv::Mat modified_image;
};


#endif
