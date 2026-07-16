#include "voxenta/effects/effect.h"
#include <imgui.h>
#include <algorithm>

#if defined(VOXENTA_EFFECTS_HOT_RELOAD)
extern "C" {
    __attribute__((visibility("default")))
    effect_list_t g_effects;
}
#else
effect_list_t g_effects;
#endif

effect::effect() {
    g_effects.emplace_back(*this);
}

std::string effect::describe_image(const cv::Mat& img) {
    if (img.empty())
        return "No image";

    const char* color_space = "Unknown";
    switch (img.channels()) {
        case 1: color_space = "Grayscale"; break;
        case 3: color_space = "BGR"; break;
        case 4: color_space = "BGRA"; break;
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "%d x %d  \xE2\x80\xA2  %s", img.cols, img.rows, color_space);
    return buf;
}

bool effect::param_slider_float(const char* label, float* value, float min, float max, bool connected, float live_value)
{
    if (connected) {
        *value = live_value;
        ImGui::BeginDisabled();
        ImGui::SliderFloat(label, value, min, max);
        ImGui::EndDisabled();
        return false;
    }
    return ImGui::SliderFloat(label, value, min, max);
}

bool effect::param_slider_int(const char* label, int* value, int min, int max, bool connected, int live_value)
{
    if (connected) {
        *value = live_value;
        ImGui::BeginDisabled();
        ImGui::SliderInt(label, value, min, max);
        ImGui::EndDisabled();
        return false;
    }
    return ImGui::SliderInt(label, value, min, max);
}

bool effect::param_checkbox(const char* label, bool* value, bool connected, bool live_value)
{
    if (connected) {
        *value = live_value;
        ImGui::BeginDisabled();
        ImGui::Checkbox(label, value);
        ImGui::EndDisabled();
        return false;
    }
    return ImGui::Checkbox(label, value);
}

bool effect::source_value_float(const char* label, float* value, bool has_range, float min, float max)
{
    if (has_range) {
        *value = std::clamp(*value, min, max);
        return ImGui::SliderFloat(label, value, min, max);
    }
    return ImGui::InputFloat(label, value);
}

bool effect::source_value_int(const char* label, int* value, bool has_range, int min, int max)
{
    if (has_range) {
        *value = std::clamp(*value, min, max);
        return ImGui::SliderInt(label, value, min, max);
    }
    return ImGui::InputInt(label, value);
}

void effect::convert_to_rgb(cv::Mat* img) {
    if (int image_type = img->type(); image_type == CV_8UC1) {
        cv::Mat converted_image;
        cv::cvtColor(*img, converted_image, cv::COLOR_GRAY2BGR);
        *img = converted_image;
    }
    else if (image_type > CV_8UC3) {
        fprintf(stderr, "unable to convert image \n");
    }
}
