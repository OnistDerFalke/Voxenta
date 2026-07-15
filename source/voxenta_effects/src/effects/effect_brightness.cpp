#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_brightness final : public effect_clonable<effect_brightness> {
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

    std::vector<pin_info> inputs() const override
    {
        return {
            { "Image",      pin_type::image },
            { "Brightness", pin_type::int_value },
        };
    }

    bool run_ui() override
    {
        return effect::param_slider_int("Brightness", &m_brightness, -255, 255, pin_connected(1), pin_int(1));
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        remember_inputs(inputs);
        cv::Mat image = inputs[0].image;
        if (image.empty())
            return { pin_value::make_image(cv::Mat()) };
        effect::convert_to_rgb(&image);

        const int brightness = inputs[1].connected ? inputs[1].i : m_brightness;

        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    final_image.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(
                        image.at<cv::Vec3b>(y, x)[c] + brightness);
                }
            }
        }
        remember_output({ pin_value::make_image(final_image) });
        return { pin_value::make_image(final_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_brightness)
