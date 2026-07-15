#include "voxenta/effects/effect.h"

#include <imgui.h>

class effect_output final : public effect_clonable<effect_output> {
    cv::Mat m_received_image;

public:
    const char* get_name() override { return "Image Output"; }
    const char* get_description() override
    {
        return "Final result of the effect pipeline, shown in the output image "
            "window. Mandatory - cannot be deleted or removed.";
    }
    bool is_internal_node() const override { return true; }
    std::vector<pin_info> outputs() const override { return {}; }

    cv::Mat get_preview_image() const override { return m_received_image; }

    bool run_ui() override
    {
        return false;
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        m_received_image = inputs[0].image;
        return {};
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_output)