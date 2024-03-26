#ifndef VOXENTA_PROCESSING_METHODS_H
#define VOXENTA_PROCESSING_METHODS_H

#include <opencv4/opencv2/opencv.hpp>
#include "imgui/imgui.h"
#include "processing_data.h"

class processing_methods {
public:
    processing_methods();
    cv::Mat run_method(cv::Mat image, int index, processing_data data);
private:
    cv::Mat brightness(cv::Mat image, int val);
    cv::Mat contrast(cv::Mat image, float val);
    cv::Mat negative(cv::Mat image);
};


#endif
