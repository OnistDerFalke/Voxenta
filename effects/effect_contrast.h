#ifndef VOXENTA_CONTRAST_H
#define VOXENTA_CONTRAST_H

#include <opencv2/core/mat.hpp>
#include "utils/data_containers.h"
#include "imgui.h"

class effect_contrast : public effect {
public:
    effect_contrast() = default;

    containers::processing_data pop_data(bool* first_frame, containers::value_container* ui_state) override
    {
        auto data = containers::processing_data();

        if(*first_frame) {
            ui_state->floats[0] = 1.0f;
            *first_frame = false;
        }
        ImGui::SliderFloat("Contrast", &ui_state->floats[0], 0, 25);
        data.values.floats[0] = ui_state->floats[0];
        data.description = const_cast<char*>(
                "Changes the contrast of an image.\n\n"
                "Parameters:\n"
                "- Contrast: Contrast coefficient. Between zero and one contrast is less. One leaves image unchanged. "
                "Values higher than one returns image with higher contrast.");
        return data;
    }

    cv::Mat pop_image(cv::Mat image, containers::processing_data data) override {
        cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
        for( int y = 0; y < image.rows; y++ ) {
            for( int x = 0; x < image.cols; x++ ) {
                for( int c = 0; c < image.channels(); c++ ) {
                    final_image.at<cv::Vec3b>(y,x)[c] =
                            cv::saturate_cast<uchar>( data.values.floats[0] * (float)(image.at<cv::Vec3b>(y,x)[c] - 128) + 128);
                }
            }
        }
        return final_image;
    }
};
#endif