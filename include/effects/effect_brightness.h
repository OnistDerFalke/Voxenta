#ifndef VOXENTA_EFFECT_BRIGHTNESS_H
#define VOXENTA_EFFECT_BRIGHTNESS_H

#include "utils/data_containers.h"
#include "imgui.h"
#include "effect.h"

#include <opencv2/core/mat.hpp>

class effect_brightness : public effect
{
public:
    effect_brightness() = default;

    containers::processing_data pop_data(bool* first_frame, containers::value_container *ui_state) override {
        auto data = containers::processing_data();
        if(*first_frame) {
            ui_state->ints[0] = 1;
            *first_frame = false;
        }
        ImGui::SliderInt("Brightness", &ui_state->ints[0], -255, 255);
        data.values.ints[0] = ui_state->ints[0];
        data.description = const_cast<char*>(
                "Changes the brightness of an image.\n\n"
                "Parameters:\n"
                "- Brightness: Brightness addition value. Positive value makes image brighter. "
                "Negative value makes image darker. Zero value leaves image unchanged.");
        return data;
    }

    cv::Mat pop_image(cv::Mat image, containers::processing_data data) override {
        cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
        for( int y = 0; y < image.rows; y++ ) {
            for( int x = 0; x < image.cols; x++ ) {
                for( int c = 0; c < image.channels(); c++ ) {
                    final_image.at<cv::Vec3b>(y,x)[c] =
                            cv::saturate_cast<uchar>( image.at<cv::Vec3b>(y,x)[c] + data.values.ints[0] );
                }
            }
        }
        return final_image;
    }
};

#endif
