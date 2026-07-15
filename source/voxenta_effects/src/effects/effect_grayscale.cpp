#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_grayscale final : public effect_clonable<effect_grayscale> {
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

    std::vector<pin_info> inputs() const override
    {
        return {
            { "Image",        pin_type::image },
            { "Red Weight",   pin_type::float_value },
            { "Green Weight", pin_type::float_value },
            { "Blue Weight",  pin_type::float_value },
        };
    }

    bool run_ui() override
    {
        bool updated = false;
#define F(fn) updated = (fn) || updated;
        F(effect::param_slider_float("Red Weight", &m_red, 0, 1, pin_connected(1), pin_float(1)));
        F(effect::param_slider_float("Green Weight", &m_green, 0, 1, pin_connected(2), pin_float(2)));
        F(effect::param_slider_float("Blue Weight", &m_blue, 0, 1, pin_connected(3), pin_float(3)));
#undef F
        return updated;
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        remember_inputs(inputs);
        cv::Mat image = inputs[0].image;
        if (image.empty())
            return { pin_value::make_image(cv::Mat()) };
        effect::convert_to_rgb(&image);

        const float red = inputs[1].connected ? inputs[1].f : m_red;
        const float green = inputs[2].connected ? inputs[2].f : m_green;
        const float blue = inputs[3].connected ? inputs[3].f : m_blue;

        cv::Mat final_image = cv::Mat::zeros(image.size(), image.type());
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                for (int c = 0; c < image.channels(); c++) {
                    float pixel_value = 0;
                    pixel_value += static_cast<float>(image.at<cv::Vec3b>(y, x)[0]) * red;
                    pixel_value += static_cast<float>(image.at<cv::Vec3b>(y, x)[1]) * green;
                    pixel_value += static_cast<float>(image.at<cv::Vec3b>(y, x)[2]) * blue;
                    final_image.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(static_cast<int>(pixel_value));
                }
            }
        }
        remember_output({ pin_value::make_image(final_image) });
        return { pin_value::make_image(final_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_grayscale)
