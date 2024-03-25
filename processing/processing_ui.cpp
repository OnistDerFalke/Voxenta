#include "processing_ui.h"
#include "processing_data.h"
#include "imgui/imgui.h"

processing_ui::processing_ui()
{
    _intMem = 0;
};

processing_data processing_ui::run_method(int index) {
    if(index == 0) {
        return brightness();
    }
}

processing_data processing_ui::brightness() {
    processing_data data = processing_data();
    ImGui::SliderInt("Brightness", &_intMem, -255, 255);
    data._intVal = _intMem;
    return data;
}