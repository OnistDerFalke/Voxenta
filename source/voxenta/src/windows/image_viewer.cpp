#include "voxenta/windows/image_viewer.h"

#include <algorithm>
#include <cstdint>
#include <GL/gl.h>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

void image_viewer::open()
{
    open_ = true;
    fit_pending_ = true;
}

void image_viewer::update_texture(const cv::Mat& img)
{
    if (img.empty())
        return;

    constexpr int kMaxViewerDim = 2048;
    cv::Mat resized;
    if (img.cols > kMaxViewerDim || img.rows > kMaxViewerDim) {
        if (img.cols >= img.rows) {
            const int w = kMaxViewerDim;
            const int h = std::max(1, static_cast<int>(static_cast<float>(img.rows) * w / img.cols));
            cv::resize(img, resized, cv::Size(w, h));
        }
        else {
            const int h = kMaxViewerDim;
            const int w = std::max(1, static_cast<int>(static_cast<float>(img.cols) * h / img.rows));
            cv::resize(img, resized, cv::Size(w, h));
        }
    }
    else {
        resized = img;
    }

    cv::Mat rgb;
    if (resized.channels() == 1)
        cv::cvtColor(resized, rgb, cv::COLOR_GRAY2RGB);
    else
        cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);

    GLuint texture;
    if (texture_ == nullptr) {
        glGenTextures(1, &texture);
        texture_ = reinterpret_cast<void*>(static_cast<intptr_t>(texture));
    }
    else {
        texture = static_cast<GLuint>(reinterpret_cast<intptr_t>(texture_));
    }

    const GLint filter = pixelated_ ? GL_NEAREST : GL_LINEAR;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rgb.cols, rgb.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.ptr());

    tex_w_ = rgb.cols;
    tex_h_ = rgb.rows;
}

void image_viewer::show(const cv::Mat& image)
{
    if (open_) {
        ImGui::OpenPopup("Image Viewer");
        open_ = false;
    }

    update_texture(image);

    const ImVec2 display_size = ImGui::GetIO().DisplaySize;
    const ImVec2 viewer_size(display_size.x * 0.68f, display_size.y * 0.78f);
    ImGui::SetNextWindowPos(ImVec2(
        (display_size.x - viewer_size.x) * 0.5f,
        (display_size.y - viewer_size.y) * 0.5f));
    ImGui::SetNextWindowSize(viewer_size);

    bool close_requested = false;
    if (ImGui::BeginPopupModal("Image Viewer", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

        ImGui::SetWindowFontScale(0.8f);
        ImGui::TextUnformatted("Image Viewer");

        {
            const float radius = ImGui::GetFontSize() * 0.5f + 2.0f;
            const float diameter = radius * 2.0f;
            ImGui::SameLine(ImGui::GetWindowWidth() - diameter - 8.0f);

            const ImVec2 top_left = ImGui::GetCursorScreenPos();
            const ImVec2 center(top_left.x + radius, top_left.y + radius);

            ImGui::InvisibleButton("##viewer_close", ImVec2(diameter, diameter));
            const bool hovered = ImGui::IsItemHovered();
            const bool active = ImGui::IsItemActive();

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            if (hovered) {
                const ImU32 bg = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive : ImGuiCol_ButtonHovered);
                draw_list->AddCircleFilled(center, radius, bg, 16);
            }

            const float cross = radius * 0.5f;
            const ImU32 cross_col = ImGui::GetColorU32(ImGuiCol_Text);
            draw_list->AddLine(ImVec2(center.x - cross, center.y - cross), ImVec2(center.x + cross, center.y + cross), cross_col, 1.5f);
            draw_list->AddLine(ImVec2(center.x - cross, center.y + cross), ImVec2(center.x + cross, center.y - cross), cross_col, 1.5f);

            if (ImGui::IsItemClicked())
                close_requested = true;
        }

        ImGui::SetWindowFontScale(1.0f);
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 12.0f));

        if (image.empty() || texture_ == nullptr) {
            ImGui::TextDisabled("No image");
        }
        else {
            ImGui::BeginChild("viewer_canvas", ImVec2(0, 0), false,
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            const ImVec2 canvas_size = ImGui::GetWindowSize();
            const ImVec2 canvas_window_pos = ImGui::GetWindowPos();

            if (ImGui::IsWindowHovered()) {
                const float wheel = ImGui::GetIO().MouseWheel;
                if (wheel != 0.0f && tex_w_ > 0 && tex_h_ > 0) {
                    const float old_zoom = zoom_;
                    const float new_zoom = std::clamp(old_zoom + wheel * 0.1f, 0.1f, 10.0f);

                    if (new_zoom != old_zoom) {
                        const ImVec2 mouse_local(
                            ImGui::GetMousePos().x - canvas_window_pos.x,
                            ImGui::GetMousePos().y - canvas_window_pos.y);

                        const ImVec2 old_center(canvas_size.x * 0.5f + pan_.x, canvas_size.y * 0.5f + pan_.y);
                        const float old_draw_w = static_cast<float>(tex_w_) * old_zoom;
                        const float old_draw_h = static_cast<float>(tex_h_) * old_zoom;
                        const ImVec2 old_top_left(old_center.x - old_draw_w * 0.5f, old_center.y - old_draw_h * 0.5f);

                        const ImVec2 img_pixel(
                            (mouse_local.x - old_top_left.x) / old_zoom,
                            (mouse_local.y - old_top_left.y) / old_zoom);

                        const ImVec2 new_top_left(
                            mouse_local.x - img_pixel.x * new_zoom,
                            mouse_local.y - img_pixel.y * new_zoom);

                        const float new_draw_w = static_cast<float>(tex_w_) * new_zoom;
                        const float new_draw_h = static_cast<float>(tex_h_) * new_zoom;
                        const ImVec2 new_center(new_top_left.x + new_draw_w * 0.5f, new_top_left.y + new_draw_h * 0.5f);

                        pan_ = ImVec2(new_center.x - canvas_size.x * 0.5f, new_center.y - canvas_size.y * 0.5f);
                        zoom_ = new_zoom;
                    }
                }
            }

            ImGui::SetCursorScreenPos(canvas_window_pos);
            ImGui::SetNextItemAllowOverlap();
            ImGui::InvisibleButton("##viewer_drag", canvas_size,
                ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

            if (ImGui::IsItemActive() &&
                (ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseDragging(ImGuiMouseButton_Right))) {
                const ImVec2 delta = ImGui::GetIO().MouseDelta;
                pan_.x += delta.x;
                pan_.y += delta.y;
            }

            if (fit_pending_ && tex_w_ > 0 && tex_h_ > 0) {
                zoom_ = std::min(
                    canvas_size.x / static_cast<float>(tex_w_),
                    canvas_size.y / static_cast<float>(tex_h_));
                pan_ = ImVec2(0.0f, 0.0f);
                fit_pending_ = false;
            }

            const float draw_w = static_cast<float>(tex_w_) * zoom_;
            const float draw_h = static_cast<float>(tex_h_) * zoom_;

            const ImVec2 center(canvas_size.x * 0.5f + pan_.x, canvas_size.y * 0.5f + pan_.y);
            const ImVec2 top_left(center.x - draw_w * 0.5f, center.y - draw_h * 0.5f);

            ImGui::GetWindowDrawList()->AddImage(texture_,
                ImVec2(canvas_window_pos.x + top_left.x, canvas_window_pos.y + top_left.y),
                ImVec2(canvas_window_pos.x + top_left.x + draw_w, canvas_window_pos.y + top_left.y + draw_h));

            const ImVec2 button_size(80.0f, 0.0f);
            const float recenter_y = canvas_window_pos.y + canvas_size.y - ImGui::GetFrameHeight() - 12.0f;
            const float filter_y = recenter_y - ImGui::GetFrameHeight() - 6.0f;
            const float button_x = canvas_window_pos.x + canvas_size.x - button_size.x - 12.0f;

            ImGui::SetWindowFontScale(0.75f);

            ImGui::SetCursorScreenPos(ImVec2(button_x, filter_y));
            if (ImGui::Button(pixelated_ ? "Pixelated" : "Filtered", button_size)) {
                pixelated_ = !pixelated_;
            }

            ImGui::SetCursorScreenPos(ImVec2(button_x, recenter_y));
            const bool recenter_clicked = ImGui::Button("Recenter", button_size);

            ImGui::SetWindowFontScale(1.0f);
            if (recenter_clicked) {
                fit_pending_ = true;
            }

            ImGui::EndChild();
        }

        if (close_requested)
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
    else {
        open_ = false;
    }
}