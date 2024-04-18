#ifndef VOXENTA_NODE_EDITOR_H
#define VOXENTA_NODE_EDITOR_H

#include "voxenta/node_editor/graph.h"

#include <string>
#include <vector>
#include <imnodes.h>
#include <GLFW/glfw3.h>
#include <cmath>


class node_editor {
public:

    enum class NodeType
    {
        add,
        multiply,
        output,
        sine,
        time,
        value
    };

    struct Node
    {
        NodeType type;
        float    value;

        explicit Node(const NodeType t) : type(t), value(0.f) {}

        Node(const NodeType t, const float v) : type(t), value(v) {}
    };

    node_editor();
    void show(ImVec2 size);
    void set_minimap_location(ImNodesMiniMapLocation location);
    ImNodesMiniMapLocation get_minimap_location() const;
private:
    enum class UiNodeType
    {
        add,
        multiply,
        output,
        sine,
        time,
    };

    struct UiNode
    {
        UiNodeType type;
        // The identifying id of the ui node. For add, multiply, sine, and time
        // this is the "operation" node id. The additional input nodes are
        // stored in the structs.
        int id;
        union
        {
            struct
            {
                int lhs, rhs;
            } add;

            struct
            {
                int lhs, rhs;
            } multiply;

            struct
            {
                int r, g, b;
            } output;

            struct
            {
                int input;
            } sine;
        } ui;
    };

    template<class T> T clamp(T x, T a, T b);
    ImU32 evaluate(const graphs::Graph<Node>& graph, int root_node);
    graphs::Graph<Node> graph_;
    std::vector<UiNode> nodes_;
    int root_node_id_;
    ImNodesMiniMapLocation minimap_location_;
    float current_time_seconds = 0.f;
    bool  emulate_three_button_mouse = false;
    float editor_scale = 0.1f;
};

#endif
