#include "processing_ui.h"
#include "processing_data.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


processing_ui::processing_ui()
{
    auto pd = processing_data();
    int slots_per_type = pd.slots_per_type;
    for(int i=0; i<slots_per_type; i++)
    {
        _intMem.push_back(0);
        _floatMem.push_back(0.0f);
        _doubleMem.push_back(0.0);
    }
};

processing_data processing_ui::run_method(int index) {
    if(index == 0) return brightness();
    if(index == 1) return contrast();
    if(index == 2) return negative();
    if(index == 3) return gaussian_blur();
}

processing_data processing_ui::brightness() {
    auto data = processing_data();
    ImGui::SliderInt("Brightness", &_intMem[0], -255, 255);
    data._intVal[0] = _intMem[0];
    data.description = const_cast<char*>(
            "Changes the brightness of an image.\n\n"
            "Solution: Final Pixel = Pixel + Brightness");
    return data;
}

processing_data processing_ui::contrast() {
    auto data = processing_data();
    ImGui::SliderFloat("Contrast", &_floatMem[0], 0, 25);
    data._floatVal[0] = _floatMem[0];
    data.description = const_cast<char*>(
            "Changes the contrast of an image.\n\n"
            "Solution: Final Pixel = (Pixel - 128) * Contrast + 128");
    return data;
}

processing_data processing_ui::negative() {
    auto data = processing_data();
    data.description = const_cast<char*>(
            "Reverts the colors of the image.\n\n"
            "Solution: Final Pixel = 255 - Pixel");
    return data;
}

processing_data processing_ui::gaussian_blur() {
    auto data = processing_data();
    auto* kernel_size = new int[2];
    kernel_size[0] = 1;
    kernel_size[1] = 1;
    auto* sigma = new float[2];
    sigma[0] = 1;
    sigma[1] = 1;
    const char* borders[] {
        const_cast<char*>("Constant"),
        const_cast<char*>("Replicate"),
        const_cast<char*>("Reflect"),
        const_cast<char*>("Reflect 101"),
        const_cast<char*>("Transparent"),
        const_cast<char*>("Isolated")
        };
    ImGui::InputInt2("Kernel Size (x,y)", kernel_size);
    ImGui::InputFloat2("Sigma (x,y)", sigma);
    ImGui::Combo("Border", &_intMem[2], borders, IM_ARRAYSIZE(borders));
    data._intVal[2] = _intMem[2];
    if(data._intVal[2] == 5) data._intVal[2] = 16; //offset for OpenCV
    _intMem[0] = kernel_size[0];
    _intMem[1] = kernel_size[1];
    data._intVal[0] = _intMem[0];
    data._intVal[1] = _intMem[1];
    _doubleMem[0] = static_cast<double>(sigma[0]);
    _doubleMem[1] = static_cast<double>(sigma[1]);
    data._doubleVal[0] = _doubleMem[0];
    data._doubleVal[1] = _doubleMem[1];
    data.description = const_cast<char*>(
            "Blurs the image using Gaussian Blur.\n\n"
            "Solution: [TO BE DONE]");
    delete[] kernel_size;
    delete[] sigma;
    return data;
}