#ifndef VOXENTA_IMAGE_VIEWER_H
#define VOXENTA_IMAGE_VIEWER_H

#include <opencv2/opencv.hpp>
#include <imgui.h>

class image_viewer {
public:
    image_viewer() = default;
    ~image_viewer();

    image_viewer(const image_viewer&) = delete;
    image_viewer& operator=(const image_viewer&) = delete;
    image_viewer(image_viewer&& other) noexcept;
    image_viewer& operator=(image_viewer&& other) noexcept;

    void open();
    bool is_open() const { return open_; }
    void show(const cv::Mat& image);

private:
    void update_texture(const cv::Mat& img);
    void release_texture();

    bool open_ = false;
    bool fit_pending_ = false;

    unsigned int texture_id_ = 0;
    int tex_w_ = 0;
    int tex_h_ = 0;
    cv::Mat cached_source_;
    bool cached_pixelated_ = false;

    float zoom_ = 1.0f;
    ImVec2 pan_ = ImVec2(0.0f, 0.0f);
    bool pixelated_ = false;
};

#endif