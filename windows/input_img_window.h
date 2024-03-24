#ifndef VOXENTA_INPUT_IMG_WINDOW_H
#define VOXENTA_INPUT_IMG_WINDOW_H

#include <cstdio>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

class input_img_window {
public:
    explicit input_img_window(ImVec2 mws); //mws -> main window size
    void set_mws(ImVec2 size);
    void show();
private:
    ImVec2 mws;
};


#endif
