#include "effects/effect.h"
#include "imgui.h"

#include <opencv2/core/mat.hpp>

class effect_negative final : effect {
public:
    const char* get_name() override
    {
        return "Negative";
    }

    const char* get_description() override
    {
        return "Inverts the color samples in the input image.";
    }

    bool run_ui() override
    {
        return false;
    }

    cv::Mat run(cv::Mat image) override
    {
        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( 255 - image.at<cv::Vec3b>(y,x)[c]);
                }
            }
        }
        return final_image;
    }
};

REGISTER_EFFECT(effect_negative)
