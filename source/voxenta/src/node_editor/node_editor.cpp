#include "voxenta/node_editor/node_editor.h"

#include "voxenta/effect_manager.h"

node_editor::node_editor() : root_node_id_(-1), minimap_location_(ImNodesMiniMapLocation_BottomRight)
{
    //Enables editor context movement with scroll button
    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;
}

void node_editor::show()
{
    ImNodes::BeginNodeEditor();
    {
        const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                                ImNodes::IsEditorHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
        if (!ImGui::IsAnyItemHovered() && open_popup)
        {
            ImGui::OpenPopup("add node");
        }

        if (ImGui::BeginPopup("add node"))
        {
            const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

            for (auto& effect : effect_manager::effects()) {
                if (ImGui::MenuItem(effect.get().get_name())) {
                    const int id = graph_.insert_node(effect.get());
                    ui_nodes_.emplace_back(id, effect);
                    ImNodes::SetNodeScreenSpacePos(id, click_pos);
                }
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    for (auto [id, effect] : ui_nodes_) {
        ImNodes::BeginNode(id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(effect.get().get_name());
        ImNodes::EndNodeTitleBar();

        effect.get().run_ui();

        ImNodes::BeginOutputAttribute(id);
        ImGui::Text("output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
    }

    for (const auto& edge : graph_.edges())
        ImNodes::Link(edge.id, edge.from, edge.to);

    ImNodes::MiniMap(0.2f, minimap_location_);
    ImNodes::EndNodeEditor();

    {
        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            const char* start_name = graph_.node(start_attr).get().get_name();
            const char* end_name = graph_.node(end_attr).get().get_name();

            const bool valid_link = start_name != end_name;
            if (valid_link)
            {
                // Ensure the edge is always directed from the value to
                // whatever produces the value
                /*if (start_type != NodeType::value)
                {
                    std::swap(start_attr, end_attr);
                }*/
                graph_.insert_edge(start_attr, end_attr);
            }
        }
    }

    // Handle deleted links

    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            graph_.erase_edge(link_id);
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedLinks();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            static std::vector<int> selected_links;
            selected_links.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedLinks(selected_links.data());
            for (const int edge_id : selected_links)
            {
                graph_.erase_edge(edge_id);
            }
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedNodes();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            selected_nodes_.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedNodes(selected_nodes_.data());
            for (const int node_id : selected_nodes_)
            {
                graph_.erase_node(node_id);
                auto iter = std::find_if(
                        ui_nodes_.begin(), ui_nodes_.end(), [node_id](const auto& x) -> bool {
                            return std::get<0>(x) == node_id;
                        });
                // Erase any additional internal nodes
                ui_nodes_.erase(iter);
            }

            ImNodes::ClearNodeSelection();
        }
    }

    // The color output window

    cv::Mat output = root_node_id_ != -1 ? evaluate(graph_, root_node_id_) : cv::Mat();
}

cv::Mat node_editor::evaluate(const graphs::Graph<std::reference_wrapper<effect>>& graph, const int root_node)
{
    std::stack<int> postorder;
    dfs_traverse(
            graph, root_node, [&postorder](const int node_id) -> void { postorder.push(node_id); });

    std::stack<cv::Mat> value_stack;
    while (!postorder.empty())
    {
        const int id = postorder.top();
        postorder.pop();
        effect& node = graph.node(id);

        //node.run();
    }
    assert(value_stack.size() == 1);
    cv::Mat output = value_stack.top();
    value_stack.pop();
    return output;
}

void node_editor::set_minimap_location(ImNodesMiniMapLocation location) {
    this->minimap_location_ = location;
}

ImNodesMiniMapLocation node_editor::get_minimap_location() const {
    return this->minimap_location_;
}
