#ifndef VOXENTA_INPUT_IMG_WINDOW_H
#define VOXENTA_INPUT_IMG_WINDOW_H

#include <imgui.h>
#include <opencv2/opencv.hpp>

class input_img_window {
public:
    explicit input_img_window(ImVec2 mws);
    void set_mws(ImVec2 size);
    void show(cv::Mat image, bool just_uploaded);
private:
    ImVec2 mws;
    void* image_texture{};
    cv::Size image_size;
    bool need_update{};
};

#endif
