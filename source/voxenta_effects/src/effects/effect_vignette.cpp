#include "voxenta/effects/effect.h"

#include <algorithm>
#include <cmath>
#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_vignette final : public effect_clonable<effect_vignette> {
    float m_strength = 0.5f;
    int   m_softness = 2;
    bool  m_invert = false;

public:
    const char* get_name() override { return "Vignette"; }

    const char* get_description() override
    {
        return "Darkens (or brightens) the corners of the image relative to its center.\n\n"
            "Parameters:\n"
            "- Strength: Intensity of the effect, from 0 (none) to 1 (full).\n"
            "- Softness: Falloff exponent - higher values give a sharper edge.\n"
            "- Invert: If true, brightens edges instead of darkening them.\n\n"
            "Strength/Softness/Invert can each be driven either by their own control "
            "here, or by connecting a Float/Int/Bool node to the matching pin.";
    }

    std::vector<pin_info> inputs() const override
    {
        return {
            { "Image",    pin_type::image },
            { "Strength", pin_type::float_value, true, 0.0f, 1.0f },
            { "Softness", pin_type::int_value, true, 1.0f, 8.0f },
            { "Invert",   pin_type::bool_value },
        };
    }

    bool run_ui() override
    {
        bool updated = false;
#define F(fn) updated = (fn) || updated;
        F(effect::param_slider_float("Strength", &m_strength, 0.0f, 1.0f, pin_connected(1), pin_float(1)));
        F(effect::param_slider_int("Softness", &m_softness, 1, 8, pin_connected(2), pin_int(2)));
        F(effect::param_checkbox("Invert", &m_invert, pin_connected(3), pin_bool(3)));
#undef F
        return updated;
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        remember_inputs(inputs);
        cv::Mat image = inputs[0].image;

        if (image.empty()) {
            remember_output({ pin_value::make_image(cv::Mat()) });
            return { pin_value::make_image(cv::Mat()) };
        }

        effect::convert_to_rgb(&image);

        const float strength = inputs[1].connected ? inputs[1].f : m_strength;
        const int   softness = inputs[2].connected ? inputs[2].i : m_softness;
        const bool  invert = inputs[3].connected ? inputs[3].b : m_invert;

        cv::Mat final_image = image.clone();
        const float cx = image.cols * 0.5f;
        const float cy = image.rows * 0.5f;
        const float max_dist = std::sqrt(cx * cx + cy * cy);

        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                const float dx = static_cast<float>(x) - cx;
                const float dy = static_cast<float>(y) - cy;
                float dist = std::sqrt(dx * dx + dy * dy) / max_dist;

                float mask = 1.0f - strength * std::pow(dist, static_cast<float>(softness));
                mask = std::clamp(mask, 0.0f, 1.0f);
                if (invert) mask = 1.0f - mask;

                for (int c = 0; c < image.channels(); c++) {
                    final_image.at<cv::Vec3b>(y, x)[c] =
                        cv::saturate_cast<uchar>(static_cast<float>(image.at<cv::Vec3b>(y, x)[c]) * mask);
                }
            }
        }

        remember_output({ pin_value::make_image(final_image) });
        return { pin_value::make_image(final_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_vignette)