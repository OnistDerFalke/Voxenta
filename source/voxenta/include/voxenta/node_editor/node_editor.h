#ifndef VOXENTA_NODE_EDITOR_H
#define VOXENTA_NODE_EDITOR_H

#include "voxenta/node_editor/graph.h"
#include "voxenta/effects/effect.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <imnodes.h>
#include <GLFW/glfw3.h>
#include <cmath>

class node_editor {
public:
    node_editor();
    void show();
    void set_minimap_location(ImNodesMiniMapLocation location);
    ImNodesMiniMapLocation get_minimap_location() const;

    void set_input_image(cv::Mat image);
    void set_input_extension(std::string extension);
    cv::Mat get_output() const;

private:
    struct ui_node {
        int node_id;
        std::unique_ptr<effect> fx;
        std::vector<int> input_attr_ids;
        std::vector<int> output_attr_ids;

        std::string custom_name;
        bool renaming = false;
        bool focus_rename = false;
        char rename_buf[128] = {};

        void* thumbnail_texture = nullptr;
        int   thumbnail_tex_w = 0;
        int   thumbnail_tex_h = 0;
    };

    struct attr_info {
        int      node_id;
        int      pin_index;
        bool     is_input;
        pin_type type;
    };

    int  add_node(effect& fx, ImVec2 screen_pos);
    void remove_node(int node_id);
    void update_node_thumbnail(ui_node& node);

    bool resolve_attr(int attr_id, attr_info& out) const;
    bool find_ui_node(int node_id, ui_node** out);
    effect* find_catalog_effect(const char* name);

    std::vector<pin_value> evaluate_node(int node_id, std::unordered_map<int, std::vector<pin_value>>& cache);
    void evaluate_and_show_output();
    void update_downstream_ranges();

    graphs::Graph<int> graph_;
    std::vector<ui_node> ui_nodes_;
    std::unordered_map<int, attr_info> attr_info_;
    int next_attr_id_ = 1'000'000;

    std::vector<int> selected_nodes_;
    int hovered_node_id_ = -1;

    ImNodesMiniMapLocation minimap_location_;
    float ui_scale_ = 1.0f;
    ImNodesStyle base_style_;
    float current_time_seconds = 0.f;

    bool initialized_ = false;
    bool view_centered_ = false;
    int input_node_id_ = -1;
    int output_node_id_ = -1;
    cv::Mat input_image_;
    std::string input_extension_;
    cv::Mat last_output_;
};

#endif