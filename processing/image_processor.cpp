#include "image_processor.h"
#include "processing_methods.h"

image_processor::image_processor()
{
    latest_index = 0;
    latest_data = processing_data();
};

cv::Mat image_processor::process_image(cv::Mat image, int index, processing_data data, bool just_uploaded) {
    was_updated = !(!just_uploaded && data.compare(latest_data) && latest_index == index);
    if(!was_updated) return latest_image;
    this->latest_index = index;
    this->latest_data = data;
    processing_methods methods = processing_methods();
    this->latest_image = methods.run_method(image, index, data);
    return latest_image;
}

bool image_processor::did_update() {
    return was_updated;
}
