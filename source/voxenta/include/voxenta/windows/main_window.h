#ifndef VOXENTA_MAIN_WINDOW_H
#define VOXENTA_MAIN_WINDOW_H

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_node_editor.h>


class main_window {
public:
    main_window();
private:
    bool window_resized(GLFWwindow* window) const;
    ImVec2 latest_size;
    bool latest_maximize_flag;
    ax::NodeEditor::EditorContext* m_context;
    static void error_callback(int error, const char* description);
    static void window_maximize_callback(GLFWwindow* window, int maximized);
};

#endif
