
#include "voxenta/node_editor/node_editor.h"

node_editor::node_editor() : graph_(), nodes_(), root_node_id_(-1), minimap_location_(ImNodesMiniMapLocation_BottomRight)
{
    //Enables editor context movement with scroll button
    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;
}


void node_editor::show(ImVec2 size)
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

            if (ImGui::MenuItem("add"))
            {
                const Node value(NodeType::value, 0.f);
                const Node op(NodeType::add);

                UiNode ui_node{};
                ui_node.type = UiNodeType::add;
                ui_node.ui.add.lhs = graph_.insert_node(value);
                ui_node.ui.add.rhs = graph_.insert_node(value);
                ui_node.id = graph_.insert_node(op);

                graph_.insert_edge(ui_node.id, ui_node.ui.add.lhs);
                graph_.insert_edge(ui_node.id, ui_node.ui.add.rhs);

                nodes_.push_back(ui_node);
                ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
            }

            if (ImGui::MenuItem("multiply"))
            {
                const Node value(NodeType::value, 0.f);
                const Node op(NodeType::multiply);

                UiNode ui_node;
                ui_node.type = UiNodeType::multiply;
                ui_node.ui.multiply.lhs = graph_.insert_node(value);
                ui_node.ui.multiply.rhs = graph_.insert_node(value);
                ui_node.id = graph_.insert_node(op);

                graph_.insert_edge(ui_node.id, ui_node.ui.multiply.lhs);
                graph_.insert_edge(ui_node.id, ui_node.ui.multiply.rhs);

                nodes_.push_back(ui_node);
                ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
            }

            if (ImGui::MenuItem("output") && root_node_id_ == -1)
            {
                const Node value(NodeType::value, 0.f);
                const Node out(NodeType::output);

                UiNode ui_node;
                ui_node.type = UiNodeType::output;
                ui_node.ui.output.r = graph_.insert_node(value);
                ui_node.ui.output.g = graph_.insert_node(value);
                ui_node.ui.output.b = graph_.insert_node(value);
                ui_node.id = graph_.insert_node(out);

                graph_.insert_edge(ui_node.id, ui_node.ui.output.r);
                graph_.insert_edge(ui_node.id, ui_node.ui.output.g);
                graph_.insert_edge(ui_node.id, ui_node.ui.output.b);

                nodes_.push_back(ui_node);
                ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
                root_node_id_ = ui_node.id;
            }

            if (ImGui::MenuItem("sine"))
            {
                const Node value(NodeType::value, 0.f);
                const Node op(NodeType::sine);

                UiNode ui_node;
                ui_node.type = UiNodeType::sine;
                ui_node.ui.sine.input = graph_.insert_node(value);
                ui_node.id = graph_.insert_node(op);

                graph_.insert_edge(ui_node.id, ui_node.ui.sine.input);

                nodes_.push_back(ui_node);
                ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
            }

            if (ImGui::MenuItem("time"))
            {
                UiNode ui_node;
                ui_node.type = UiNodeType::time;
                ui_node.id = graph_.insert_node(Node(NodeType::time));

                nodes_.push_back(ui_node);
                ImNodes::SetNodeScreenSpacePos(ui_node.id, click_pos);
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    for (const UiNode& node : nodes_)
    {
        switch (node.type)
        {
            case UiNodeType::add:
            {
                const float node_width = 100.f;
                ImNodes::BeginNode(node.id);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("add");
                ImNodes::EndNodeTitleBar();
                {
                    ImNodes::BeginInputAttribute(node.ui.add.lhs);
                    const float label_width = ImGui::CalcTextSize("left").x;
                    ImGui::TextUnformatted("left");
                    if (graph_.num_edges_from_node(node.ui.add.lhs) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat("##hidelabel", &graph_.node(node.ui.add.lhs).value, 0.01f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                {
                    ImNodes::BeginInputAttribute(node.ui.add.rhs);
                    const float label_width = ImGui::CalcTextSize("right").x;
                    ImGui::TextUnformatted("right");
                    if (graph_.num_edges_from_node(node.ui.add.rhs) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat("##hidelabel", &graph_.node(node.ui.add.rhs).value, 0.01f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                ImGui::Spacing();

                {
                    ImNodes::BeginOutputAttribute(node.id);
                    const float label_width = ImGui::CalcTextSize("result").x;
                    ImGui::Indent(node_width - label_width);
                    ImGui::TextUnformatted("result");
                    ImNodes::EndOutputAttribute();
                }

                ImNodes::EndNode();
            }
                break;
            case UiNodeType::multiply:
            {
                const float node_width = 100.0f;
                ImNodes::BeginNode(node.id);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("multiply");
                ImNodes::EndNodeTitleBar();

                {
                    ImNodes::BeginInputAttribute(node.ui.multiply.lhs);
                    const float label_width = ImGui::CalcTextSize("left").x;
                    ImGui::TextUnformatted("left");
                    if (graph_.num_edges_from_node(node.ui.multiply.lhs) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat(
                                "##hidelabel", &graph_.node(node.ui.multiply.lhs).value, 0.01f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                {
                    ImNodes::BeginInputAttribute(node.ui.multiply.rhs);
                    const float label_width = ImGui::CalcTextSize("right").x;
                    ImGui::TextUnformatted("right");
                    if (graph_.num_edges_from_node(node.ui.multiply.rhs) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat(
                                "##hidelabel", &graph_.node(node.ui.multiply.rhs).value, 0.01f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                ImGui::Spacing();

                {
                    ImNodes::BeginOutputAttribute(node.id);
                    const float label_width = ImGui::CalcTextSize("result").x;
                    ImGui::Indent(node_width - label_width);
                    ImGui::TextUnformatted("result");
                    ImNodes::EndOutputAttribute();
                }

                ImNodes::EndNode();
            }
                break;
            case UiNodeType::output:
            {
                const float node_width = 100.0f;
                ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(11, 109, 191, 255));
                ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(45, 126, 194, 255));
                ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(81, 148, 204, 255));
                ImNodes::BeginNode(node.id);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("output");
                ImNodes::EndNodeTitleBar();

                ImGui::Dummy(ImVec2(node_width, 0.f));
                {
                    ImNodes::BeginInputAttribute(node.ui.output.r);
                    const float label_width = ImGui::CalcTextSize("r").x;
                    ImGui::TextUnformatted("r");
                    if (graph_.num_edges_from_node(node.ui.output.r) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat(
                                "##hidelabel", &graph_.node(node.ui.output.r).value, 0.01f, 0.f, 1.0f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                ImGui::Spacing();

                {
                    ImNodes::BeginInputAttribute(node.ui.output.g);
                    const float label_width = ImGui::CalcTextSize("g").x;
                    ImGui::TextUnformatted("g");
                    if (graph_.num_edges_from_node(node.ui.output.g) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat(
                                "##hidelabel", &graph_.node(node.ui.output.g).value, 0.01f, 0.f, 1.f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                ImGui::Spacing();

                {
                    ImNodes::BeginInputAttribute(node.ui.output.b);
                    const float label_width = ImGui::CalcTextSize("b").x;
                    ImGui::TextUnformatted("b");
                    if (graph_.num_edges_from_node(node.ui.output.b) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat(
                                "##hidelabel", &graph_.node(node.ui.output.b).value, 0.01f, 0.f, 1.0f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }
                ImNodes::EndNode();
                ImNodes::PopColorStyle();
                ImNodes::PopColorStyle();
                ImNodes::PopColorStyle();
            }
                break;
            case UiNodeType::sine:
            {
                const float node_width = 100.0f;
                ImNodes::BeginNode(node.id);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("sine");
                ImNodes::EndNodeTitleBar();

                {
                    ImNodes::BeginInputAttribute(node.ui.sine.input);
                    const float label_width = ImGui::CalcTextSize("number").x;
                    ImGui::TextUnformatted("number");
                    if (graph_.num_edges_from_node(node.ui.sine.input) == 0ull)
                    {
                        ImGui::SameLine();
                        ImGui::PushItemWidth(node_width - label_width);
                        ImGui::DragFloat(
                                "##hidelabel",
                                &graph_.node(node.ui.sine.input).value,
                                0.01f,
                                0.f,
                                1.0f);
                        ImGui::PopItemWidth();
                    }
                    ImNodes::EndInputAttribute();
                }

                ImGui::Spacing();

                {
                    ImNodes::BeginOutputAttribute(node.id);
                    const float label_width = ImGui::CalcTextSize("output").x;
                    ImGui::Indent(node_width - label_width);
                    ImGui::TextUnformatted("output");
                    ImNodes::EndOutputAttribute();
                }

                ImNodes::EndNode();
            }
                break;
            case UiNodeType::time:
            {
                ImNodes::BeginNode(node.id);

                ImNodes::BeginNodeTitleBar();
                ImGui::TextUnformatted("time");
                ImNodes::EndNodeTitleBar();

                ImNodes::BeginOutputAttribute(node.id);
                ImGui::Text("output");
                ImNodes::EndOutputAttribute();

                ImNodes::EndNode();
            }
                break;
        }
    }

    for (const auto& edge : graph_.edges())
    {
        // If edge doesn't start at value, then it's an internal edge, i.e.
        // an edge which links a node's operation to its input. We don't
        // want to render node internals with visible links.
        if (graph_.node(edge.from).type != NodeType::value)
            continue;

        ImNodes::Link(edge.id, edge.from, edge.to);
    }

    ImNodes::MiniMap(0.2f, minimap_location_);
    ImNodes::EndNodeEditor();

    // Handle new links
    // These are driven by Imnodes, so we place the code after EndNodeEditor().

    {
        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            const NodeType start_type = graph_.node(start_attr).type;
            const NodeType end_type = graph_.node(end_attr).type;

            const bool valid_link = start_type != end_type;
            if (valid_link)
            {
                // Ensure the edge is always directed from the value to
                // whatever produces the value
                if (start_type != NodeType::value)
                {
                    std::swap(start_attr, end_attr);
                }
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
            static std::vector<int> selected_nodes;
            selected_nodes.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedNodes(selected_nodes.data());
            for (const int node_id : selected_nodes)
            {
                graph_.erase_node(node_id);
                auto iter = std::find_if(
                        nodes_.begin(), nodes_.end(), [node_id](const UiNode& node) -> bool {
                            return node.id == node_id;
                        });
                // Erase any additional internal nodes
                switch (iter->type)
                {
                    case UiNodeType::add:
                        graph_.erase_node(iter->ui.add.lhs);
                        graph_.erase_node(iter->ui.add.rhs);
                        break;
                    case UiNodeType::multiply:
                        graph_.erase_node(iter->ui.multiply.lhs);
                        graph_.erase_node(iter->ui.multiply.rhs);
                        break;
                    case UiNodeType::output:
                        graph_.erase_node(iter->ui.output.r);
                        graph_.erase_node(iter->ui.output.g);
                        graph_.erase_node(iter->ui.output.b);
                        root_node_id_ = -1;
                        break;
                    case UiNodeType::sine:
                        graph_.erase_node(iter->ui.sine.input);
                        break;
                    default:
                        break;
                }
                nodes_.erase(iter);
            }
        }
    }

    // The color output window

    const ImU32 color =
            root_node_id_ != -1 ? evaluate(graph_, root_node_id_) : IM_COL32(255, 20, 147, 255);
}

ImU32 node_editor::evaluate(const graphs::Graph<Node>& graph, const int root_node)
{
    std::stack<int> postorder;
    dfs_traverse(
            graph, root_node, [&postorder](const int node_id) -> void { postorder.push(node_id); });

    std::stack<float> value_stack;
    while (!postorder.empty())
    {
        const int id = postorder.top();
        postorder.pop();
        const Node node = graph.node(id);

        switch (node.type)
        {
            case NodeType::add:
            {
                const float rhs = value_stack.top();
                value_stack.pop();
                const float lhs = value_stack.top();
                value_stack.pop();
                value_stack.push(lhs + rhs);
            }
                break;
            case NodeType::multiply:
            {
                const float rhs = value_stack.top();
                value_stack.pop();
                const float lhs = value_stack.top();
                value_stack.pop();
                value_stack.push(rhs * lhs);
            }
                break;
            case NodeType::sine:
            {
                const float x = value_stack.top();
                value_stack.pop();
                const float res = std::abs(std::sin(x));
                value_stack.push(res);
            }
                break;
            case NodeType::time:
            {
                value_stack.push(current_time_seconds);
            }
                break;
            case NodeType::value:
            {
                // If the edge does not have an edge connecting to another node, then just use the value
                // at this node. It means the node's input pin has not been connected to anything and
                // the value comes from the node's UI.
                if (graph.num_edges_from_node(id) == 0ull)
                {
                    value_stack.push(node.value);
                }
            }
                break;
            default:
                break;
        }
    }

    // The final output node isn't evaluated in the loop -- instead we just pop
    // the three values which should be in the stack.
    assert(value_stack.size() == 3ull);
    const int b = static_cast<int>(255.f * clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
    value_stack.pop();
    const int g = static_cast<int>(255.f * clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
    value_stack.pop();
    const int r = static_cast<int>(255.f * clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
    value_stack.pop();

    return IM_COL32(r, g, b, 255);
}

void node_editor::set_minimap_location(ImNodesMiniMapLocation location) {
    this->minimap_location_ = location;
}

ImNodesMiniMapLocation node_editor::get_minimap_location() const {
    return this->minimap_location_;
}

template<class T> T node_editor::clamp(T x, T a, T b)
{
    return std::min(b, std::max(x, a));
}