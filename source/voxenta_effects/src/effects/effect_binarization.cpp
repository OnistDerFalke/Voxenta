#include "voxenta/effects/effect.h"

#include <array>

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_binarization final : public effect_clonable<effect_binarization> {
    enum binarization_method_t {
        METHOD_MANUAL,
        METHOD_MANUAL_INV,
        METHOD_MANUAL_TO_ZERO,
        METHOD_MANUAL_TO_ZERO_INV,
        METHOD_MANUAL_TRUNC,
        METHOD_AUTO_OTSU,
        METHOD_AUTO_TRIANGLE,

        NUM_METHOD
    };

    enum binarization_method_param_t {
        PARAM_THRESHOLD = 1 << 0,
        PARAM_MAXVALUE  = 1 << 1,
        PARAM_GRAYSCALE = 1 << 2,
    };

    std::array<cv::ThresholdTypes, NUM_METHOD> binarization_method_types = {
        cv::THRESH_BINARY,
        cv::THRESH_BINARY_INV,
        cv::THRESH_TRUNC,
        cv::THRESH_TOZERO,
        cv::THRESH_TOZERO_INV,
        cv::THRESH_OTSU,
        cv::THRESH_TRIANGLE
    };

    std::array<uint, NUM_METHOD> binarization_method_params = {
        PARAM_THRESHOLD | PARAM_MAXVALUE | PARAM_GRAYSCALE,
        PARAM_THRESHOLD | PARAM_MAXVALUE | PARAM_GRAYSCALE,
        PARAM_THRESHOLD | PARAM_GRAYSCALE,
        PARAM_THRESHOLD | PARAM_GRAYSCALE,
        PARAM_THRESHOLD | PARAM_GRAYSCALE,
        PARAM_MAXVALUE,
        PARAM_MAXVALUE
    };

    std::array<const char*, NUM_METHOD> binarization_method_names = {
        "Manual",
        "Manual Inverted",
        "Manual To Zero",
        "Manual To Zero Inverted",
        "Manual Trunc",
        "Automated Otsu",
        "Automated Triangle"
    };

    std::array<const char*, NUM_METHOD> binarization_method_descriptions = {
        "Changes grayscale image to binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Threshold: Pixels above threshold have max value, otherwise black.\n"
        "- Max Value: Value of a pixel that initial value was higher than threshold.\n"
        "- No Grayscale: If true, initial image is not converted to grayscale before binarization.",

        "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Threshold: Pixel above this value is black, otherwise is max value.\n"
        "- Max Value: Value of a pixel that initial value was lower or equal the threshold.\n"
        "- No Grayscale: If true, initial image is not converted to grayscale before binarization.",

        "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Threshold: Pixel above threshold value is left unchanged, otherwise is black.\n"
        "- No Grayscale: If true, initial image is not converted to grayscale before binarization.",

        "Changes grayscale image to binary image with dynamically computed threshold."
        " If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Threshold: Pixel above threshold value is black, otherwise is left unchanged.\n"
        "- No Grayscale: If true, initial image is not converted to grayscale before binarization.",

        "Changes grayscale image to inverted binary color image using manually chosen threshold."
        " If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Threshold: Pixel above this value has threshold value, otherwise is left unchanged.\n"
        "- No Grayscale: If true, initial image is not converted to grayscale before binarization.",

        "Changes grayscale image to binarized with automated threshold computed using Otsu method."
        " If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Max Value: Value of a pixel that initial value was lower or equal the threshold.",

        "Changes grayscale image to binarized with automated threshold computed using Triangle method."
        " If image is not in grayscale it converts it to this form.\n\n"
        "Parameters:\n"
        "- Max Value: Value of a pixel that initial value was lower or equal the threshold."
    };

    binarization_method_t m_method = METHOD_MANUAL;
    int m_threshold = 127;
    int m_maxvalue = 255;
    bool m_nograyscale = false;

public:
    const char* get_name() override
    {
        return "Binarization";
    }

    const char* get_description() override
    {
        return binarization_method_descriptions[m_method];
    }

    std::vector<pin_info> inputs() const override
    {
        return {
            { "Image",         pin_type::image },
            { "Threshold",     pin_type::int_value, true, 0.0f, 255.0f },
            { "Max Value",     pin_type::int_value, true, 0.0f, 255.0f },
            { "No Grayscale",  pin_type::bool_value },
        };
    }

    bool run_ui() override
    {
        bool updated = false;

#define F(fn) updated = (fn) || updated;

        F(ImGui::Combo("Method",
            reinterpret_cast<int*>(&m_method),
            binarization_method_names.data(),
            binarization_method_names.size()));

        const auto flags = binarization_method_params[m_method];
        if (flags & PARAM_THRESHOLD) {
            F(effect::param_slider_int("Threshold", &m_threshold, 0, 255, pin_connected(1), pin_int(1)));
        }
        if (flags & PARAM_MAXVALUE) {
            F(effect::param_slider_int("Max Value", &m_maxvalue, 0, 255, pin_connected(2), pin_int(2)));
        }
        if (flags & PARAM_GRAYSCALE) {
            F(effect::param_checkbox("No Grayscale (experimental)", &m_nograyscale, pin_connected(3), pin_bool(3)));
        }

#undef F

        return updated;
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        remember_inputs(inputs);
        cv::Mat image = inputs[0].image;
        if (image.empty())
            return { pin_value::make_image(cv::Mat()) };

        cv::Mat final_image, grayscale_image;
        effect::convert_to_rgb(&image);
        cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);

        const auto flags = binarization_method_params[m_method];

        const int  threshold = inputs[1].connected ? inputs[1].i : m_threshold;
        const int  maxvalue = inputs[2].connected ? inputs[2].i : m_maxvalue;
        const bool nograyscale = inputs[3].connected ? inputs[3].b : m_nograyscale;

        if (flags & PARAM_GRAYSCALE && nograyscale)
            grayscale_image = image;

        int thresh = (flags & PARAM_THRESHOLD) ? threshold : 0;
        int maxval = (flags & PARAM_MAXVALUE) ? maxvalue : 0;
        int type = binarization_method_types[m_method];

        cv::threshold(grayscale_image, final_image, thresh, maxval, type);

        remember_output({ pin_value::make_image(final_image) });
        return { pin_value::make_image(final_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_binarization)
