#ifndef VOXENTA_EFFECT_NEGATIVE_H
#define VOXENTA_EFFECT_NEGATIVE_H

#include <opencv2/core/mat.hpp>
#include "utils/data_containers.h"
#include "imgui.h"

class effect_negative : public effect{
public:
    effect_negative() = default;

    containers::processing_data pop_data(bool* first_frame, containers::value_container* ui_state) override
    {
        auto data = containers::processing_data();
        data.description = const_cast<char*>(
                "Reverts the colors of the image.");
        return data;
    }

    cv::Mat pop_image(cv::Mat image, containers::processing_data data) override {
        cv::Mat final_image = cv::Mat::zeros( image.size(), image.type() );;
        for( int y = 0; y < image.rows; y++ ) {
            for( int x = 0; x < image.cols; x++ ) {
                for( int c = 0; c < image.channels(); c++ ) {
                    final_image.at<cv::Vec3b>(y,x)[c] =
                            cv::saturate_cast<uchar>( 255 - image.at<cv::Vec3b>(y,x)[c]);
                }
            }
        }
        return final_image;
    }
};

#endif
