#ifndef VOXENTA_IMAGE_PROCESSOR_H
#define VOXENTA_IMAGE_PROCESSOR_H

#include <opencv4/opencv2/opencv.hpp>
#include "utils/data_containers.h"

class image_processor {
public:
    image_processor();
    cv::Mat process_image(cv::Mat image, int index, containers::processing_data data, bool just_uploaded);
    bool did_update() const;
private:
    containers::processing_data latest_data{};
    int latest_index;
    cv::Mat latest_image;
    bool was_updated{};
};


#endif
