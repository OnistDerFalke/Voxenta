
#ifndef VOXENTA_EFFECT_H
#define VOXENTA_EFFECT_H

#include "utils/data_containers.h"

#include <opencv2/opencv.hpp>


class effect {
public:
    effect() = default;
    virtual containers::processing_data pop_data(bool* first_frame, containers::value_container* ui_state) = 0;
    virtual cv::Mat pop_image(cv::Mat image, containers::processing_data data) = 0;
};
#endif
