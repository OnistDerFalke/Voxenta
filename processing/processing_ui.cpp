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
        _boolMem.push_back(false);
    }
    last_index = -1;

    //processing functions pointers
    functions.push_back(&processing_ui::brightness);
    functions.push_back(&processing_ui::contrast);
    functions.push_back(&processing_ui::negative);
    functions.push_back(&processing_ui::grayscale);
    functions.push_back(&processing_ui::binarization);
    functions.push_back(&processing_ui::gaussian_blur);
};

processing_data processing_ui::run_method(int index) {
    if(index != last_index)
        first_frame = true;
    last_index = index;
   return (this->*functions[index])();
}

processing_data processing_ui::brightness() {
    auto data = processing_data();
    if(first_frame) {
        _intMem[0] = 1;
        first_frame = false;
    }
    ImGui::SliderInt("Brightness", &_intMem[0], -255, 255);
    data._intVal[0] = _intMem[0];
    data.description = const_cast<char*>(
            "Changes the brightness of an image.\n\n"
            "Parameters:\n"
            "- Brightness: Brightness addition value. Positive value makes image brighter. "
            "Negative value makes image darker. Zero value leaves image unchanged.");
    return data;
}

processing_data processing_ui::contrast() {
    auto data = processing_data();
    if(first_frame) {
        _floatMem[0] = 1.0f;
        first_frame = false;
    }
    ImGui::SliderFloat("Contrast", &_floatMem[0], 0, 25);
    data._floatVal[0] = _floatMem[0];
    data.description = const_cast<char*>(
            "Changes the contrast of an image.\n\n"
            "Parameters:\n"
            "- Contrast: Contrast coefficient. Between zero and one contrast is less. One leaves image unchanged. "
            "Values higher than one returns image with higher contrast.");
    return data;
}

processing_data processing_ui::negative() {
    auto data = processing_data();
    data.description = const_cast<char*>(
            "Reverts the colors of the image.");
    return data;
}

processing_data processing_ui::grayscale() {
    auto data = processing_data();
    if(first_frame) {
        _floatMem[0] = 0.2126f;
        _floatMem[1] = 0.7152f;
        _floatMem[2] = 0.0722f;
        first_frame = false;
    }
    ImGui::SliderFloat("Red Weight", &_floatMem[0], 0, 1);
    ImGui::SliderFloat("Green Weight", &_floatMem[1], 0, 1);
    ImGui::SliderFloat("Blue Weight", &_floatMem[2], 0, 1);
    float weight_sum = 0;
    for(int i=0; i<3; i++) {
       weight_sum+=_floatMem[i];
    }
    for(int i=0; i<3; i++) {
        _floatMem[i] = _floatMem[i]/weight_sum;
        data._floatVal[i] = _floatMem[i];
    }
    data.description = const_cast<char*>(
            "Changes RGB image to grayscale with specific weight parameters.\n\n"
            "Parameters:\n"
            "- Red Weight: Weight for red channel in final grayscale image.\n"
            "- Green Weight: Weight for green channel in final grayscale image.\n"
            "- Blue Weight: Weight for blue channel in final grayscale image.");
    return data;
}

processing_data processing_ui::binarization() {
    auto data = processing_data();
    if(first_frame) {
        _intMem[0] = 0;
        _intMem[1] = 127;
        _intMem[2] = 255;
        _boolMem[0] = false;
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
    ImGui::Combo("Method", &_intMem[0],
                 binarization_method, IM_ARRAYSIZE(binarization_method));
    if(_intMem[0] == 0)
    {
        ImGui::SliderInt("Threshold", &_intMem[1], 0, 255);
        ImGui::SliderInt("Max Value", &_intMem[2], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[1] = _intMem[1];
        data._intVal[2] = _intMem[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixels above threshold have max value, otherwise black.\n"
                "- Max Value: Value of a pixel that initial value was higher than threshold.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(_intMem[0] == 1)
    {
        ImGui::SliderInt("Threshold", &_intMem[1], 0, 255);
        ImGui::SliderInt("Max Value", &_intMem[2], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[1] = _intMem[1];
        data._intVal[2] = _intMem[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixel above this value is black, otherwise is max value.\n"
                "- Max Value: Value of a pixel that initial value was lower or equal the threshold.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(_intMem[0] == 2)
    {
        ImGui::SliderInt("Threshold", &_intMem[1], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[1] = _intMem[1];
        data.description = const_cast<char*>(
                "Changes grayscale image to inverted binary color image using manually chosen threshold. If image is not in grayscale it converts it to this form\n\n."
                "Parameters:\n"
                "- Threshold: Pixel above threshold value is left unchanged, otherwise is black.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(_intMem[0] == 3)
    {
        ImGui::SliderInt("Threshold", &_intMem[1], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[1] = _intMem[1];
        data.description = const_cast<char*>(
                "Changes grayscale image to binary image with dynamically computed threshold."
                "If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixel above threshold value is black, otherwise is left unchanged.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(_intMem[0] == 4)
    {
        ImGui::SliderInt("Threshold", &_intMem[1], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[1] = _intMem[1];
        data.description = const_cast<char*>(
                "Changes grayscale image to inverted binary color image using manually chosen threshold."
                " If image is not in grayscale it converts it to this form.\n\n"
                "Parameters:\n"
                "- Threshold: Pixel above this value has threshold value, otherwise is left unchanged.\n"
                "- No Grayscale: If true, initial image is not converted to grayscale before binarization.");
    }
    if(_intMem[0] == 5)
    {
        ImGui::SliderInt("Max Value", &_intMem[2], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[2] = _intMem[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to binarized with automated threshold computed using Otsu method."
                " If image is not in grayscale it converts it to this form.");
    }
    if(_intMem[0] == 6)
    {
        ImGui::SliderInt("Max Value", &_intMem[2], 0, 255);
        data._intVal[0] = _intMem[0];
        data._intVal[2] = _intMem[2];
        data.description = const_cast<char*>(
                "Changes grayscale image to binarized with automated threshold computed using Triangle method."
                " If image is not in grayscale it converts it to this form.");
    }

    if(_intMem[0] != 5 && _intMem[0] != 6)
    {
        bool no_grayscale = _boolMem[0];
        ImGui::Checkbox("No Grayscale (experimental)", &no_grayscale);
        _boolMem[0] = no_grayscale;
        data._boolVal[0] = _boolMem[0];
    }
    return data;
}

processing_data processing_ui::gaussian_blur() {
    auto data = processing_data();
    if(first_frame) {
        _intMem[0] = 1;
        _intMem[1] = 1;
        _floatMem[0] = 0.0f;
        _floatMem[1] = 0.0f;
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
    ImGui::InputInt2("Kernel Size (x,y)", _intMem.data());
    ImGui::InputFloat2("Sigma (x,y)", _floatMem.data());
    ImGui::Combo("Border", &_intMem[2], borders, IM_ARRAYSIZE(borders));
    data._intVal[2] = _intMem[2];
    if(data._intVal[2] == 5) data._intVal[2] = 16; //offset for OpenCV
    if(_intMem[0] == 0 || _intMem[0] == 0) //if one is zero also seconds has to be and it runs sigma computations
    {
        _intMem[0] = 0;
        _intMem[1] = 0;
        if(_floatMem[0] == 0 || _floatMem[1] == 0) {
            _floatMem[0] = 1.0f;
            _floatMem[1] = 1.0f;
        }
    }
    if(_intMem[0]<0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size X cannot be negative.");
        _intMem[0] = 1;
    }
    if(_intMem[1]<0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size Y cannot be negative.");
        _intMem[1] = 1;
    }
    if(_intMem[0]%2==0 && _intMem[0]!=0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size X must be odd.");
        _intMem[0] = 1;
    }
    if(_intMem[1]%2==0 &&  _intMem[1]!=0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kernel size Y must be odd.");
        _intMem[1] = 1;
    }
    for(int i=0; i<2; i++)
    {
        if(_intMem[i]>kernel_limit) _intMem[i] = (int)kernel_limit-1;
        if(_floatMem[i]>sigma_limit) _floatMem[i] = sigma_limit-1;
    }
    data._intVal[0] = _intMem[0];
    data._intVal[1] = _intMem[1];
    data._doubleVal[0] = static_cast<double>(_floatMem[0]);
    data._doubleVal[1] = static_cast<double>(_floatMem[1]);
    data.description = const_cast<char*>(
            "Blurs an image using a Gaussian filter.\n\n"
            "Parameters:\n"
            "- Kernel Size: Size of a kernel that runs on whole image applying Gaussian filtering effect. "
            "It must be positive and odd.\n"
            "- Sigma: Gaussian kernel standard deviation in horizontal (X) and vertical (Y) directions. "
            "If Sigma Y is zero, it gets the value from Sigma X. If both are zero, effect is computed from kernel size.");
    return data;
}
