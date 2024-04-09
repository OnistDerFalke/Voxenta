
#ifndef VOXENTA_EFFECT_BINARIZATION_H
#define VOXENTA_EFFECT_BINARIZATION_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include "utils/data_containers.h"
#include "imgui.h"

class effect_binarization : public effect {
public:
    effect_binarization() = default;

    containers::processing_data pop_data(bool* first_frame, containers::value_container* ui_state) override
    {
        auto data = containers::processing_data();
        if(*first_frame) {
            ui_state->ints[0] = 0;
            ui_state->ints[1] = 127;
            ui_state->ints[2] = 255;
            ui_state->bools[0] = false;
            *first_frame = false;
        }
        const char* binarization_method[] {
                const_cast<char*>("Manual"),
                const_cast<char*>("Manual Inverted"),
                const_cast<char*>("Manual To Zero"),
                const_cast<char*>("Manual To Zero Inverted"),
                const_cast<char*>("Manual Trunc"),
                const_cast<char*>("Automated Otsu"),
                const_cast<char*>("Automated Triangle")};
        ImGui::Combo("Method", &ui_state->ints[0],
                     binarization_method, IM_ARRAYSIZE(binarization_method));
        if(ui_state->ints[0] == 0)
        {
            ImGui::SliderInt("Threshold", &ui_state->ints[1], 0, 255);
            ImGui::SliderInt("Max Value", &ui_state->ints[2], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[1] = ui_state->ints[1];
            data.values.ints[2] = ui_state->ints[2];
            data.description = const_cast<char*>(
                    "Changes grayscale image to binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
                    "Parameters:\n"
                    "- Threshold: Pixels above threshold have max value, otherwise black.\n"
                    "- Max Value: Value of a pixel that initial value was higher than threshold.\n"
                    "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
        }
        if(ui_state->ints[0] == 1)
        {
            ImGui::SliderInt("Threshold", &ui_state->ints[1], 0, 255);
            ImGui::SliderInt("Max Value", &ui_state->ints[2], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[1] = ui_state->ints[1];
            data.values.ints[2] = ui_state->ints[2];
            data.description = const_cast<char*>(
                    "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
                    "Parameters:\n"
                    "- Threshold: Pixel above this value is black, otherwise is max value.\n"
                    "- Max Value: Value of a pixel that initial value was lower or equal the threshold.\n"
                    "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
        }
        if(ui_state->ints[0] == 2)
        {
            ImGui::SliderInt("Threshold", &ui_state->ints[1], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[1] = ui_state->ints[1];
            data.description = const_cast<char*>(
                    "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form\n\n."
                    "Parameters:\n"
                    "- Threshold: Pixel above threshold value is left unchanged, otherwise is black.\n"
                    "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
        }
        if(ui_state->ints[0] == 3)
        {
            ImGui::SliderInt("Threshold", &ui_state->ints[1], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[1] = ui_state->ints[1];
            data.description = const_cast<char*>(
                    "Changes grayscale image to binary image with dynamically computed threshold."
                    "If image is not in grayscale it converts it to this form.\n\n"
                    "Parameters:\n"
                    "- Threshold: Pixel above threshold value is black, otherwise is left unchanged.\n"
                    "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
        }
        if(ui_state->ints[0] == 4)
        {
            ImGui::SliderInt("Threshold", &ui_state->ints[1], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[1] = ui_state->ints[1];
            data.description = const_cast<char*>(
                    "Changes grayscale image to inverted binary color image using manually chosen threshold."
                    " If image is not in grayscale it converts it to this form.\n\n"
                    "Parameters:\n"
                    "- Threshold: Pixel above this value has threshold value, otherwise is left unchanged.\n"
                    "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
        }
        if(ui_state->ints[0] == 5)
        {
            ImGui::SliderInt("Max Value", &ui_state->ints[2], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[2] = ui_state->ints[2];
            data.description = const_cast<char*>(
                    "Changes grayscale image to binarized with automated threshold computed using Otsu method."
                    " If image is not in grayscale it converts it to this form.");
        }
        if(ui_state->ints[0] == 6)
        {
            ImGui::SliderInt("Max Value", &ui_state->ints[2], 0, 255);
            data.values.ints[0] = ui_state->ints[0];
            data.values.ints[2] = ui_state->ints[2];
            data.description = const_cast<char*>(
                    "Changes grayscale image to binarized with automated threshold computed using Triangle method."
                    " If image is not in grayscale it converts it to this form.");
        }

        if(ui_state->ints[0] != 5 && ui_state->ints[0] != 6)
        {
            bool no_grayscale = ui_state->bools[0];
            ImGui::Checkbox("No Grayscale (experimental)", &no_grayscale);
            ui_state->bools[0] = no_grayscale;
            data.values.bools[0] = ui_state->bools[0];
        }
        return data;
    }

    cv::Mat pop_image(cv::Mat image, containers::processing_data data) override {
        cv::Mat final_image, grayscale_image;
        cv::cvtColor(image, grayscale_image, cv::COLOR_BGR2GRAY);
        if(data.values.bools[0]) grayscale_image = image;
        int mode = data.values.ints[0];

        if(mode == 0) cv::threshold(grayscale_image, final_image, data.values.ints[1], data.values.ints[2], cv::THRESH_BINARY);
        if(mode == 1) cv::threshold(grayscale_image, final_image, data.values.ints[1], data.values.ints[2], cv::THRESH_BINARY_INV);
        if(mode == 2) cv::threshold(grayscale_image, final_image, data.values.ints[1], 0, cv::THRESH_TOZERO);
        if(mode == 3) cv::threshold(grayscale_image, final_image, data.values.ints[1], 0, cv::THRESH_TOZERO_INV);
        if(mode == 4) cv::threshold(grayscale_image, final_image, data.values.ints[1], 0, cv::THRESH_TRUNC);
        if(mode == 5) cv::threshold(grayscale_image, final_image, 0, data.values.ints[2], cv::THRESH_OTSU);
        if(mode == 6) cv::threshold(grayscale_image, final_image, 0, data.values.ints[2], cv::THRESH_TRIANGLE);
        return final_image;
    }
};

#endif
