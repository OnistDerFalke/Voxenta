#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_brightness final : effect {
    int m_brightness = 0;

public:
    const char* get_name() override
    {
        return "Brightness";
    }

    const char* get_description() override
    {
        return "Changes the brightness of an image.\n\n"
               "Parameters:\n"
               "- Brightness: Brightness addition value. Positive value makes image brighter. "
               "Negative value makes image darker. Zero value leaves image unchanged.";
    }

    bool run_ui() override
    {
        return ImGui::SliderInt("Brightness", &m_brightness, -255, 255);
    }

    cv::Mat run(cv::Mat image) override
    {
        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(
                        image.at<cv::Vec3b>(y,x)[c] + m_brightness);
                }
            }
        }
        return final_image;
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_brightness)
