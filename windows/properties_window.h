#ifndef VOXENTA_PROPERTIES_WINDOW_H
#define VOXENTA_PROPERTIES_WINDOW_H

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_filedialog.h"

class properties_window {
public:
    properties_window(ImVec2 mws);
    void set_mws(ImVec2 size);
    void show();
private:
    ImVec2 mws;
    bool m_fileDialogOpen;
    ImFileDialogInfo m_fileDialogInfo;
    bool load_image();
};


#endif
