#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_grayscale final : effect {
    float m_red = 0.2126f;
    float m_green = 0.7152f;
    float m_blue = 0.0722f;

public:
    const char* get_name() override
    {
        return "Grayscale";
    }

    const char* get_description() override
    {
        return "Changes RGB image to grayscale with specific weight parameters.\n\n"
               "Parameters:\n"
               "- Red Weight: Weight for red channel in final grayscale image.\n"
               "- Green Weight: Weight for green channel in final grayscale image.\n"
               "- Blue Weight: Weight for blue channel in final grayscale image.";
    }

    bool run_ui() override
    {
        bool updated = false;

#define F(fn) updated = (fn) || updated;

        F(ImGui::SliderFloat("Red Weight", &m_red, 0, 1));
        F(ImGui::SliderFloat("Green Weight", &m_green, 0, 1));
        F(ImGui::SliderFloat("Blue Weight", &m_blue, 0, 1));

#undef F

        return updated;
    }

    cv::Mat run(cv::Mat image) override
    {
        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    float pixel_value = 0;
                    pixel_value += static_cast<float>(image.at<cv::Vec3b>(y, x)[0]) * m_red;
                    pixel_value += static_cast<float>(image.at<cv::Vec3b>(y, x)[1]) * m_green;
                    pixel_value += static_cast<float>(image.at<cv::Vec3b>(y, x)[2]) * m_blue;
                    final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(static_cast<int>(pixel_value));
                }
            }
        }
        return final_image;
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_grayscale)
