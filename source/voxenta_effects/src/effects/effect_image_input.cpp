#include "voxenta/effects/effect.h"

#include <imgui.h>

class effect_image_input final : public effect_clonable<effect_image_input> {
    cv::Mat m_current_image;

public:
    const char* get_name() override { return "Image Input"; }
    const char* get_description() override
    {
        return "The image currently loaded in the application. Mandatory starting "
            "point of every effect chain - cannot be deleted or removed, and its "
            "image cannot be changed here (use File > Load in the application).";
    }
    bool is_internal_node() const override { return true; }
    std::vector<pin_info> inputs()  const override { return {}; }
    std::vector<pin_info> outputs() const override { return { { "Image", pin_type::image } }; }

    void set_external_image(const cv::Mat& img) override { m_current_image = img; }
    cv::Mat get_preview_image() const override { return m_current_image; }

    bool run_ui() override
    {
        return false;
    }

    std::vector<pin_value> run(const std::vector<pin_value>&) override
    {
        return { pin_value::make_image(m_current_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_image_input)