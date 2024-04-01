#include "processing_ui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


processing_ui::processing_ui()
{
    this->ui_state = containers::value_container();
    last_index = -1;

    //processing functions pointers
    functions.push_back(&processing_ui::brightness);
    functions.push_back(&processing_ui::contrast);
    functions.push_back(&processing_ui::negative);
    functions.push_back(&processing_ui::grayscale);
    functions.push_back(&processing_ui::binarization);
    functions.push_back(&processing_ui::gaussian_blur);
};

containers::processing_data processing_ui::run_method(int index) {
    if(index != last_index)
        first_frame = true;
    last_index = index;
    return (this->*functions[index])();
}

containers::processing_data processing_ui::brightness() {
    auto data = containers::processing_data();
    if(first_frame) {
        ui_state.ints[0] = 1;
        first_frame = false;
    }
    ImGui::SliderInt("Brightness", &ui_state.ints[0], -255, 255);
    data.values.ints[0] = ui_state.ints[0];
    data.description = const_cast<char*>(
            "Changes the brightness of an image.\n\n"
            "Parameters:\n"
            "- Brightness: Brightness addition value. Positive value makes image brighter. "
            "Negative value makes image darker. Zero value leaves image unchanged.");
    return data;
}

containers::processing_data processing_ui::contrast() {
    auto data = containers::processing_data();
    if(first_frame) {
        ui_state.floats[0] = 1.0f;
        first_frame = false;
    }
    ImGui::SliderFloat("Contrast", &ui_state.floats[0], 0, 25);
    data.values.floats[0] = ui_state.floats[0];
    data.description = const_cast<char*>(
            "Changes the contrast of an image.\n\n"
            "Parameters:\n"
            "- Contrast: Contrast coefficient. Between zero and one contrast is less. One leaves image unchanged. "
            "Values higher than one returns image with higher contrast.");
    return data;
}

containers::processing_data processing_ui::negative() {
    auto data = containers::processing_data();
    data.description = const_cast<char*>(
            "Reverts the colors of the image.");
    return data;
}

containers::processing_data processing_ui::grayscale() {
    auto data = containers::processing_data();
    if(first_frame) {
        ui_state.floats[0] = 0.2126f;
        ui_state.floats[1] = 0.7152f;
        ui_state.floats[2] = 0.0722f;
        first_frame = false;
    }
    ImGui::SliderFloat("Red Weight", &ui_state.floats[0], 0, 1);
    ImGui::SliderFloat("Green Weight", &ui_state.floats[1], 0, 1);
    ImGui::SliderFloat("Blue Weight", &ui_state.floats[2], 0, 1);
    float weight_sum = 0;
    for(int i=0; i<3; i++) {
       weight_sum+=ui_state.floats[i];
    }
    for(int i=0; i<3; i++) {
        ui_state.floats[i] = ui_state.floats[i] / weight_sum;
        data.values.floats[i] = ui_state.floats[i];
    }
    data.description = const_cast<char*>(
            "Changes RGB image to grayscale with specific weight parameters.\n\n"
            "Parameters:\n"
            "- Red Weight: Weight for red channel in final grayscale image.\n"
            "- Green Weight: Weight for green channel in final grayscale image.\n"
            "- Blue Weight: Weight for blue channel in final grayscale image.");
    return data;
}

containers::processing_data processing_ui::binarization() {
    auto data = containers::processing_data();
    if(first_frame) {
        ui_state.ints[0] = 0;
        ui_state.ints[1] = 127;
        ui_state.ints[2] = 255;
        ui_state.bools[0] = false;
        first_frame = false;
    }
    const char* binarization_method[] {
            const_cast<char*>("Manual"),
            const_cast<char*>("Manual Inverted"),
            const_cast<char*>("Manual To Zero"),
            const_cast<char*>("Manual To Zero Inverted"),
            const_cast<char*>("Manual Trunc"),
            const_cast<char*>("Automated Otsu"),
            const_cast<char*>("Automated Triangle")};
    ImGui::Combo("Method", &ui_state.ints[0],
                 binarization_method, IM_ARRAYSIZE(binarization_method));
    if(ui_state.ints[0] == 0)
    {
        ImGui::SliderInt("Threshold", &ui_state.ints[1], 0, 255);
        ImGui::SliderInt("Max Value", &ui_state.ints[2], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[1] = ui_state.ints[1];
        data.values.ints[2] = ui_state.ints[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixels above threshold have max value, otherwise black.\n"
                "- Max Value: Value of a pixel that initial value was higher than threshold.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(ui_state.ints[0] == 1)
    {
        ImGui::SliderInt("Threshold", &ui_state.ints[1], 0, 255);
        ImGui::SliderInt("Max Value", &ui_state.ints[2], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[1] = ui_state.ints[1];
        data.values.ints[2] = ui_state.ints[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixel above this value is black, otherwise is max value.\n"
                "- Max Value: Value of a pixel that initial value was lower or equal the threshold.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(ui_state.ints[0] == 2)
    {
        ImGui::SliderInt("Threshold", &ui_state.ints[1], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[1] = ui_state.ints[1];
        data.description = const_cast<char*>(
                "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form\n\n."
                "Parameters:\n"
                "- Threshold: Pixel above threshold value is left unchanged, otherwise is black.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(ui_state.ints[0] == 3)
    {
        ImGui::SliderInt("Threshold", &ui_state.ints[1], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[1] = ui_state.ints[1];
        data.description = const_cast<char*>(
                "Changes grayscale image to binary image with dynamically computed threshold."
                "If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixel above threshold value is black, otherwise is left unchanged.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(ui_state.ints[0] == 4)
    {
        ImGui::SliderInt("Threshold", &ui_state.ints[1], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[1] = ui_state.ints[1];
        data.description = const_cast<char*>(
                "Changes grayscale image to inverted binary color image using manually chosen threshold."
                " If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixel above this value has threshold value, otherwise is left unchanged.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(ui_state.ints[0] == 5)
    {
        ImGui::SliderInt("Max Value", &ui_state.ints[2], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[2] = ui_state.ints[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to binarized with automated threshold computed using Otsu method."
                " If image is not in grayscale it converts it to this form.");
    }
    if(ui_state.ints[0] == 6)
    {
        ImGui::SliderInt("Max Value", &ui_state.ints[2], 0, 255);
        data.values.ints[0] = ui_state.ints[0];
        data.values.ints[2] = ui_state.ints[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to binarized with automated threshold computed using Triangle method."
                " If image is not in grayscale it converts it to this form.");
    }

    if(ui_state.ints[0] != 5 && ui_state.ints[0] != 6)
    {
        bool no_grayscale = ui_state.bools[0];
        ImGui::Checkbox("No Grayscale (experimental)", &no_grayscale);
        ui_state.bools[0] = no_grayscale;
        data.values.bools[0] = ui_state.bools[0];
    }
    return data;
}

containers::processing_data processing_ui::gaussian_blur() {
    auto data = containers::processing_data();
    if(first_frame) {
        ui_state.ints[0] = 1;
        ui_state.ints[1] = 1;
        ui_state.floats[0] = 0.0f;
        ui_state.floats[1] = 0.0f;
        first_frame = false;
    }
    int kernel_limit = 10000;
    float sigma_limit = 10000.0f;
    const char* borders[] {
        const_cast<char*>("Constant"),
        const_cast<char*>("Replicate"),
        const_cast<char*>("Reflect"),
        const_cast<char*>("Reflect 101"),
        const_cast<char*>("Transparent"),
        const_cast<char*>("Isolated")};
    ImGui::InputInt2("Kernel Size (x,y)", ui_state.ints.data());
    ImGui::InputFloat2("Sigma (x,y)", ui_state.floats.data());
    ImGui::Combo("Border", &ui_state.ints[2], borders, IM_ARRAYSIZE(borders));
    data.values.ints[2] = ui_state.ints[2];
    if(data.values.ints[2] == 5) data.values.ints[2] = 16; //offset for OpenCV
    if(ui_state.ints[0] == 0 || ui_state.ints[0] == 0) //if one is zero also seconds has to be and it runs sigma computations
    {
        ui_state.ints[0] = 0;
        ui_state.ints[1] = 0;
        if(ui_state.floats[0] == 0 || ui_state.floats[1] == 0) {
            ui_state.floats[0] = 1.0f;
            ui_state.floats[1] = 1.0f;
        }
    }
    if(ui_state.ints[0] < 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size X cannot be negative.");
        ui_state.ints[0] = 1;
    }
    if(ui_state.ints[1] < 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size Y cannot be negative.");
        ui_state.ints[1] = 1;
    }
    if(ui_state.ints[0] % 2 == 0 && ui_state.ints[0] != 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size X must be odd.");
        ui_state.ints[0] = 1;
    }
    if(ui_state.ints[1] % 2 == 0 && ui_state.ints[1] != 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size Y must be odd.");
        ui_state.ints[1] = 1;
    }
    for(int i=0; i<2; i++)
    {
        if(ui_state.ints[i] > kernel_limit) ui_state.ints[i] = (int)kernel_limit - 1;
        if(ui_state.floats[i] > sigma_limit) ui_state.floats[i] = sigma_limit - 1;
    }
    data.values.ints[0] = ui_state.ints[0];
    data.values.ints[1] = ui_state.ints[1];
    data.values.doubles[0] = static_cast<double>(ui_state.floats[0]);
    data.values.doubles[1] = static_cast<double>(ui_state.floats[1]);
    data.description = const_cast<char*>(
            "Blurs an image using a Gaussian filter.\n\n"
            "Parameters:\n"
            "- Kernel Size: Size of a kernel that runs on whole image applying Gaussian filtering effect. "
            "It must be positive and odd.\n"
            "- Sigma: Gaussian kernel standard deviation in horizontal (X) and vertical (Y) directions. "
            "If Sigma Y is zero, it gets the value from Sigma X. If both are zero, effect is computed from kernel size.");
    return data;
}
