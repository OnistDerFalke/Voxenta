#ifndef VOXENTA_PROCESSING_METHODS_H
#define VOXENTA_PROCESSING_METHODS_H

#include "utils/data_containers.h"

#include <opencv4/opencv2/opencv.hpp>

class processing_methods {
public:
    processing_methods();
    cv::Mat run_method(cv::Mat image, int index, containers::processing_data data);
private:
    std::vector<std::variant<std::function<cv::Mat(cv::Mat, containers::processing_data)>>> functions;
};

#endif
