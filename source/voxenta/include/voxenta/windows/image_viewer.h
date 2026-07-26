#ifndef VOXENTA_IMAGE_VIEWER_H
#define VOXENTA_IMAGE_VIEWER_H

#include <opencv2/opencv.hpp>
#include <imgui.h>

class image_viewer {
public:
    image_viewer() = default;

    void open();
    bool is_open() const { return open_; }
    void show(const cv::Mat& image);

private:
    void update_texture(const cv::Mat& img);

    bool open_ = false;
    bool fit_pending_ = false;

    void* texture_ = nullptr;
    int tex_w_ = 0;
    int tex_h_ = 0;

    float zoom_ = 1.0f;
    ImVec2 pan_ = ImVec2(0.0f, 0.0f);
    bool pixelated_ = false;
};

#endif