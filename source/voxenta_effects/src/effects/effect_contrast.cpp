#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_contrast final : public effect_clonable<effect_contrast> {
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

    std::vector<pin_info> inputs() const override
    {
        return {
            { "Image",    pin_type::image },
            { "Contrast", pin_type::float_value, true, 0.0f, 25.0f },
        };
    }

    bool run_ui() override
    {
        return effect::param_slider_float("Contrast", &m_contrast, 0, 25, pin_connected(1), pin_float(1));
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        remember_inputs(inputs);
        cv::Mat image = inputs[0].image;
        if (image.empty())
            return { pin_value::make_image(cv::Mat()) };
        effect::convert_to_rgb(&image);

        const float contrast = inputs[1].connected ? inputs[1].f : m_contrast;

        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    final_image.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(
                        contrast * static_cast<float>(image.at<cv::Vec3b>(y, x)[c] - 128) + 128);
                }
            }
        }
        remember_output({ pin_value::make_image(final_image) });
        return { pin_value::make_image(final_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_contrast)
