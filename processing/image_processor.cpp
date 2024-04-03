#include "image_processor.h"

#include <utility>
#include "processing_methods.h"

image_processor::image_processor()
{
    latest_index = 0;
    latest_data = containers::processing_data();
};

/* Returns the modified image from base image*/
cv::Mat image_processor::process_image(cv::Mat image, int index, containers::processing_data data, bool just_uploaded) {

    //If nothing changed do not apply effect (without it - it may be a problem for a CPU)
    was_updated = !(!just_uploaded && data.compare(latest_data) && latest_index == index);
    if(!was_updated) return latest_image;

    //Saving latest index and data to know (in the next frame) if something changed
    this->latest_index = index;
    this->latest_data = data;

    //Running the effect method on base image and returning modified
    processing_methods methods = processing_methods();
    this->latest_image = methods.run_method(std::move(image), index, data);
    return latest_image;
}

/* Returns if effect or it's properties changed */
bool image_processor::did_update() const {
    return was_updated;
}
