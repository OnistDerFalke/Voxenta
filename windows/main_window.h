#ifndef VOXENTA_MAIN_WINDOW_H
#define VOXENTA_MAIN_WINDOW_H

#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "input_img_window.h"
#include "output_img_window.h"
#include "properties_window.h"
#include "win_prop.h"

class main_window {
public:
    main_window();
private:
    bool window_resized(GLFWwindow* window) const;
    ImVec2 latest_size;
    bool latest_maximize_flag;
    static void error_callback(int error, const char* description);
    static void window_maximize_callback(GLFWwindow* window, int maximized);
};


#endif //VOXENTA_MAIN_WINDOW_H
