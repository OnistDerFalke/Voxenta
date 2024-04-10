#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_contrast final : effect {
    float m_contrast = 1.0f;

public:
    const char* get_name() override
    {
        return "Contrast";
    }

    const char* get_description() override
    {
        return "Changes the contrast of an image.\n\n"
               "Parameters:\n"
               "- Contrast: Contrast coefficient. Between zero and one contrast is less. One leaves image unchanged. "
               "Values higher than one returns image with higher contrast.";
    }

    bool run_ui() override
    {
        return ImGui::SliderFloat("Contrast", &m_contrast, 0, 25);
    }

    cv::Mat run(cv::Mat image) override
    {
        effect::convert_to_rgb(&image);

        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    final_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(
                                m_contrast * static_cast<float>(image.at<cv::Vec3b>(y, x)[c] - 128) + 128);
                }
            }
        }
        return final_image;
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_contrast)
