#include "voxenta/effects/effect.h"

#include <array>

#include <imgui.h>
#include <opencv2/core/mat.hpp>

class effect_binarization final : public effect {
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
        "- Max Value: Value of a pixel that initial value was lower or equal the threshold.",

        "Changes grayscale image to binarized with automated threshold computed using Triangle method."
        " If image is not in grayscale it converts it to this form.\n\n"
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
            F(ImGui::SliderInt("Threshold", &m_threshold, 0, 255));
        }
        if (flags & PARAM_MAXVALUE) {
            F(ImGui::SliderInt("Max Value", &m_maxvalue, 0, 255));
        }
        if (flags & PARAM_GRAYSCALE) {
            F(ImGui::Checkbox("No Grayscale (experimental)", &m_nograyscale));
        }

#undef F

        return updated;
    }

    cv::Mat run(cv::Mat image) override
    {
        cv::Mat final_image, grayscale_image;
        cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);

        const auto flags = binarization_method_params[m_method];

        if(flags & PARAM_GRAYSCALE && m_nograyscale)
            grayscale_image = image;

        int thresh = (flags & PARAM_THRESHOLD) ? m_threshold : 0;
        int maxval = (flags & PARAM_MAXVALUE) ? m_maxvalue : 0;
        int type = binarization_method_types[m_method];

        cv::threshold(grayscale_image, final_image, thresh, maxval, type);

        return final_image;
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_binarization)
