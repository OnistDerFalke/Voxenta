#ifndef VOXENTA_ABOUT_WINDOW_H
#define VOXENTA_ABOUT_WINDOW_H

#include <imgui.h>

class about_window {
public:
    about_window();
    void show(bool* open, ImVec2 mws);
};

#endif
