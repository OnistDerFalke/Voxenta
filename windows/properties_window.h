#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include <cstdio>
#include <opencv4/opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_filedialog.h"

class properties_window {
public:
    explicit properties_window(ImVec2 mws);
    void set_mws(ImVec2 size);
    void show();
private:
    ImVec2 mws;
    bool m_fileDialogOpen;
    ImFileDialogInfo m_fileDialogInfo;
    bool load_image();
    std::filesystem::path filePath;
};


#endif
