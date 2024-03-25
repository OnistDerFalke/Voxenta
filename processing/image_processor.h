#ifndef VOXENTA_IMAGE_PROCESSOR_H
#define VOXENTA_IMAGE_PROCESSOR_H

#include <opencv4/opencv2/opencv.hpp>
#include "processing_data.h"

class image_processor {
public:
    image_processor();
    cv::Mat process_image(cv::Mat image, int index, processing_data data);
private:
};


#endif
