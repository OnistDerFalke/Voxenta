#ifndef VOXENTA_NODE_EDITOR_H
#define VOXENTA_NODE_EDITOR_H

#include "voxenta/node_editor/graph.h"
#include "voxenta/effects/effect.h"

#include <string>
#include <vector>
#include <imnodes.h>
#include <GLFW/glfw3.h>
#include <cmath>


class node_editor {
public:
    node_editor();
    void show();
    void set_minimap_location(ImNodesMiniMapLocation location);
    ImNodesMiniMapLocation get_minimap_location() const;

private:
    cv::Mat evaluate(const graphs::Graph<std::reference_wrapper<effect>>& graph, int root_node);
    graphs::Graph<std::reference_wrapper<effect>> graph_;
    std::vector<std::tuple<int, std::reference_wrapper<effect>>> ui_nodes_;
    std::vector<int> selected_nodes_;
    int root_node_id_;
    ImNodesMiniMapLocation minimap_location_;
    float current_time_seconds = 0.f;
};

#endif
