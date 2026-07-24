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

bool effect::param_slider_float(const char* label, float* value, float min, float max, bool connected, float live_value)
{
    const float wrap_width = ImGui::CalcItemWidth();
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrap_width);
    if (connected) ImGui::TextDisabled("%s", label);
    else ImGui::TextUnformatted(label);
    ImGui::PopTextWrapPos();

    const std::string id = std::string("##") + label;
    if (connected) {
        *value = live_value;
        ImGui::BeginDisabled();
        ImGui::SliderFloat(id.c_str(), value, min, max);
        ImGui::EndDisabled();
        return false;
    }
    return ImGui::SliderFloat(id.c_str(), value, min, max);
}

bool effect::param_slider_int(const char* label, int* value, int min, int max, bool connected, int live_value)
{
    const float wrap_width = ImGui::CalcItemWidth();
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrap_width);
    if (connected) ImGui::TextDisabled("%s", label);
    else ImGui::TextUnformatted(label);
    ImGui::PopTextWrapPos();

    const std::string id = std::string("##") + label;
    if (connected) {
        *value = live_value;
        ImGui::BeginDisabled();
        ImGui::SliderInt(id.c_str(), value, min, max);
        ImGui::EndDisabled();
        return false;
    }
    return ImGui::SliderInt(id.c_str(), value, min, max);
}

bool effect::param_checkbox(const char* label, bool* value, bool connected, bool live_value)
{
    const float wrap_width = ImGui::CalcItemWidth();
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrap_width);
    if (connected) ImGui::TextDisabled("%s", label);
    else ImGui::TextUnformatted(label);
    ImGui::PopTextWrapPos();

    const std::string id = std::string("##") + label;
    if (connected) {
        *value = live_value;
        ImGui::BeginDisabled();
        ImGui::Checkbox(id.c_str(), value);
        ImGui::EndDisabled();
        return false;
    }
    return ImGui::Checkbox(id.c_str(), value);
}

std::string effect::describe_resolution(const cv::Mat& img)
{
    if (img.empty())
        return "No image";
    char buf[32];
    snprintf(buf, sizeof(buf), "%d x %d", img.cols, img.rows);
    return buf;
}

std::string effect::describe_color_space(const cv::Mat& img)
{
    if (img.empty())
        return "-";
    switch (img.channels()) {
    case 1: return "Grayscale";
    case 3: return "BGR";
    case 4: return "BGRA";
    default: return "Unknown";
    }
}

bool effect::source_value_float(const char* label, float* value, bool has_range, float min, float max)
{
    const std::string id = std::string("##") + label;
    if (has_range) {
        *value = std::clamp(*value, min, max);
        return ImGui::SliderFloat(id.c_str(), value, min, max);
    }
    return ImGui::InputFloat(id.c_str(), value);
}

bool effect::source_value_int(const char* label, int* value, bool has_range, int min, int max)
{
    const std::string id = std::string("##") + label;
    if (has_range) {
        *value = std::clamp(*value, min, max);
        return ImGui::SliderInt(id.c_str(), value, min, max);
    }
    return ImGui::InputInt(id.c_str(), value);
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
