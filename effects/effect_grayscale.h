#ifndef VOXENTA_EFFECT_GRAYSCALE_H
#define VOXENTA_EFFECT_GRAYSCALE_H

#include <opencv2/core/mat.hpp>
#include "utils/data_containers.h"
#include "imgui.h"

class effect_grayscale : public effect {
public:
    effect_grayscale() = default;

    containers::processing_data pop_data(bool* first_frame, containers::value_container* ui_state) override
    {
        auto data = containers::processing_data();
        if(*first_frame) {
            ui_state->floats[0] = 0.2126f;
            ui_state->floats[1] = 0.7152f;
            ui_state->floats[2] = 0.0722f;
            *first_frame = false;
        }
        ImGui::SliderFloat("Red Weight", &ui_state->floats[0], 0, 1);
        ImGui::SliderFloat("Green Weight", &ui_state->floats[1], 0, 1);
        ImGui::SliderFloat("Blue Weight", &ui_state->floats[2], 0, 1);
        float weight_sum = 0;
        for(int i=0; i<3; i++) {
            weight_sum+=ui_state->floats[i];
        }
        for(int i=0; i<3; i++) {
            ui_state->floats[i] = ui_state->floats[i] / weight_sum;
            data.values.floats[i] = ui_state->floats[i];
        }
        data.description = const_cast<char*>(
                "Changes RGB image to grayscale with specific weight parameters.\n\n"
                "Parameters:\n"
                "- Red Weight: Weight for red channel in final grayscale image.\n"
                "- Green Weight: Weight for green channel in final grayscale image.\n"
                "- Blue Weight: Weight for blue channel in final grayscale image.");
        return data;
    }

    cv::Mat pop_image(cv::Mat image, containers::processing_data data) override {
        cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
        for( int y = 0; y < image.rows; y++ ) {
            for( int x = 0; x < image.cols; x++ ) {
                for( int c = 0; c < image.channels(); c++ ) {
                    float pixel_value = 0;
                    for(int i = 0; i < 3; i++)
                        pixel_value += (float)image.at<cv::Vec3b>(y,x)[i] * data.values.floats[i];
                    final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( (int)pixel_value);
                }
            }
        }
        return final_image;
    }
};
#endif
