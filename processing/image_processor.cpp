#include "image_processor.h"
#include "processing_methods.h"

image_processor::image_processor() = default;

cv::Mat image_processor::process_image(cv::Mat image, int index, processing_data data) {
    processing_methods methods = processing_methods();
    return methods.run_method(image, index, data);
}
