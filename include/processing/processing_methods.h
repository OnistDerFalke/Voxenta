#ifndef VOXENTA_PROCESSING_METHODS_H
#define VOXENTA_PROCESSING_METHODS_H

#include "utils/data_containers.h"

#include <opencv4/opencv2/opencv.hpp>

class processing_methods {
public:
    processing_methods();
    cv::Mat run_method(cv::Mat image, int index, containers::processing_data data);
private:
    std::vector<cv::Mat (processing_methods::*)(cv::Mat, containers::processing_data)> functions;

    //processing methods
    cv::Mat brightness(cv::Mat image, containers::processing_data data);
    cv::Mat contrast(cv::Mat image, containers::processing_data data);
    cv::Mat negative(cv::Mat image, containers::processing_data data);
    cv::Mat grayscale(cv::Mat image, containers::processing_data data);
    cv::Mat binarization(cv::Mat image, containers::processing_data data);
};

#endif
