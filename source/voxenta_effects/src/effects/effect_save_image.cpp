#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <nfd.hpp>

class effect_save_image final : public effect_clonable<effect_save_image> {
    cv::Mat m_received_image;

public:
    const char* get_name() override { return "Save Image"; }
    const char* get_description() override
    {
        return "Saves the connected image to disk. Click Save to pick a "
            "destination file - same as File > Save in the application.";
    }
    std::vector<pin_info> outputs() const override { return {}; }

    cv::Mat get_preview_image() const override { return m_received_image; }

    bool run_ui() override
    {
        if (ImGui::Button("Save...") && !m_received_image.empty()) {
            NFD::UniquePathU8 out_path;
            constexpr nfdfilteritem_t filters[2] = {
                { "PNG Image", "png" },
                { "JPEG Image", "jpg,jpeg,jfif" }
            };

            const nfdresult_t result = NFD::SaveDialog(out_path, filters, IM_ARRAYSIZE(filters));

            if (result == NFD_OKAY && out_path != nullptr) {
                cv::imwrite(out_path.get(), m_received_image);
            }
            else if (result == NFD_ERROR) {
                fprintf(stderr, "nfd error: %s\n", NFD::GetError());
            }
        }
        return false;
    }

    std::vector<pin_value> run(const std::vector<pin_value>& inputs) override
    {
        m_received_image = inputs[0].image;
        remember_output({ pin_value::make_image(m_received_image) });
        return {};
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_save_image)