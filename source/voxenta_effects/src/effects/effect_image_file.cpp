#include "voxenta/effects/effect.h"

#include <imgui.h>
#include <nfd.hpp>

class effect_image_file final : public effect_clonable<effect_image_file> {
    cv::Mat m_image;

public:
    const char* get_name() override { return "Load Image"; }
    const char* get_description() override
    {
        return "Loads a chosen image from disk and outputs it. Use this to bring "
            "extra images into the graph (e.g. for a future blend/overlay "
            "effect), separately from the application's main loaded image.";
    }
    std::vector<pin_info> inputs()  const override { return {}; }
    std::vector<pin_info> outputs() const override { return { { "Image", pin_type::image } }; }

    cv::Mat get_preview_image() const override { return m_image; }

    bool run_ui() override
    {
        bool updated = false;

        if (ImGui::Button("Load...")) {
            NFD::UniquePathU8 out_path;
            constexpr nfdfilteritem_t filters[1] = { { "Images", "jpg,jpeg,jfif,png" } };

            if (NFD::OpenDialog(out_path, filters, IM_ARRAYSIZE(filters)) == NFD_OKAY && out_path != nullptr) {
                cv::Mat loaded = cv::imread(out_path.get());
                if (loaded.data != nullptr) {
                    m_image = loaded;
                    updated = true;

                    const std::string path_str = out_path.get();
                    const size_t dot_pos = path_str.find_last_of('.');
                    remember_source_extension(dot_pos != std::string::npos ? path_str.substr(dot_pos) : "");
                }
                else {
                    fprintf(stderr, "invalid file format\n");
                }
            }
        }

        return updated;
    }

    std::vector<pin_value> run(const std::vector<pin_value>&) override
    {
        return { pin_value::make_image(m_image) };
    }
};

#include "../effect_common.inl"
REGISTER_EFFECT(effect_image_file)