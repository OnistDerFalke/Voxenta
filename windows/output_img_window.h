#ifndef VOXENTA_OUTPUT_IMG_WINDOW_H
#define VOXENTA_OUTPUT_IMG_WINDOW_H

#include <cstdio>
#include <cmath>
#include <opencv4/opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

class output_img_window {
public:
    explicit output_img_window(ImVec2 mws);
    void set_mws(ImVec2 size);
    void show(cv::Mat image, bool just_uploaded);
private:
    ImVec2 mws;
    void* image_texture{};
    cv::Size image_size;
    bool need_update{};
};



#endif
