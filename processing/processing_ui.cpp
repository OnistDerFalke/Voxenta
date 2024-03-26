#include "processing_ui.h"
#include "processing_data.h"
#include "imgui/imgui.h"

processing_ui::processing_ui()
{
    _intMem = 0;
    _floatMem = 0;
};

processing_data processing_ui::run_method(int index) {
    if(index == 0) return brightness();
    if(index == 1) return contrast();
    if(index == 2) return negative();
}

processing_data processing_ui::brightness() {
    processing_data data = processing_data();
    ImGui::SliderInt("Brightness", &_intMem, -255, 255);
    data._intVal = _intMem;
    data.description = const_cast<char*>(
            "Changes the brightness of an image.\n\n"
            "Solution: Final Pixel = Pixel + Brightness");
    return data;
}

processing_data processing_ui::contrast() {
    processing_data data = processing_data();
    ImGui::SliderFloat("Contrast", &_floatMem, 0, 25);
    data._floatVal = _floatMem;
    data.description = const_cast<char*>(
            "Changes the contrast of an image.\n\n"
            "Solution: Final Pixel = (Pixel - 128) * Contrast + 128");
    return data;
}

processing_data processing_ui::negative() {
    processing_data data = processing_data();
    data.description = const_cast<char*>(
            "Reverts the colors of the image.\n\n"
            "Solution: Final Pixel = 255 - Pixel");
    return data;
}
