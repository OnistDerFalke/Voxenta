#include "voxenta/node_editor/node_editor.h"

#include "voxenta/effect_manager.h"

#include <cstring>

namespace {
    constexpr float kNodeContentWidth = 140.0f;
}

node_editor::node_editor() : minimap_location_(ImNodesMiniMapLocation_BottomRight)
{
    ImNodes::GetIO().AltMouseButton = ImGuiMouseButton_Right;
    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;
}

effect* node_editor::find_catalog_effect(const char* name)
{
    for (auto& fx : effect_manager::effects())
        if (std::strcmp(fx.get().get_name(), name) == 0)
            return &fx.get();
    return nullptr;
}

int node_editor::add_node(effect& fx, ImVec2 screen_pos)
{
    const int node_id = graph_.insert_node(0);

    ui_node node;
    node.node_id = node_id;
    node.fx = fx.clone();

    const auto in_pins = node.fx->inputs();
    const auto out_pins = node.fx->outputs();

    for (size_t i = 0; i < in_pins.size(); ++i) {
        const int attr_id = next_attr_id_++;
        node.input_attr_ids.push_back(attr_id);
        attr_info_[attr_id] = { node_id, static_cast<int>(i), true, in_pins[i].type };
    }
    for (size_t i = 0; i < out_pins.size(); ++i) {
        const int attr_id = next_attr_id_++;
        node.output_attr_ids.push_back(attr_id);
        attr_info_[attr_id] = { node_id, static_cast<int>(i), false, out_pins[i].type };
    }

    ui_nodes_.push_back(std::move(node));
    ImNodes::SetNodeScreenSpacePos(node_id, screen_pos);
    return node_id;
}

void node_editor::remove_node(int node_id)
{
    if (node_id == input_node_id_ || node_id == output_node_id_)
        return;

    auto iter = std::find_if(ui_nodes_.begin(), ui_nodes_.end(),
        [node_id](const ui_node& n) { return n.node_id == node_id; });
    if (iter == ui_nodes_.end())
        return;

    for (int a : iter->input_attr_ids)  attr_info_.erase(a);
    for (int a : iter->output_attr_ids) attr_info_.erase(a);

    graph_.erase_node(node_id);
    ui_nodes_.erase(iter);
}

bool node_editor::resolve_attr(int attr_id, attr_info& out) const
{
    auto iter = attr_info_.find(attr_id);
    if (iter == attr_info_.end())
        return false;
    out = iter->second;
    return true;
}

bool node_editor::find_ui_node(int node_id, ui_node** out)
{
    auto iter = std::find_if(ui_nodes_.begin(), ui_nodes_.end(),
        [node_id](const ui_node& n) { return n.node_id == node_id; });
    if (iter == ui_nodes_.end())
        return false;
    *out = &(*iter);
    return true;
}

void node_editor::show()
{
    update_downstream_ranges();

    ImNodes::BeginNodeEditor();
    {
        if (!initialized_)
        {
            if (effect* img_in = find_catalog_effect("Image Input"))
                input_node_id_ = add_node(*img_in, ImVec2(60, 200));
            if (effect* img_out = find_catalog_effect("Image Output"))
                output_node_id_ = add_node(*img_out, ImVec2(500, 200));
            initialized_ = true;
        }

        const ImVec2 right_drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        constexpr float drag_threshold = 4.0f;

        const bool mouse_released = ImGui::IsMouseReleased(ImGuiMouseButton_Right);
        const bool x_check = fabsf(right_drag_delta.x) < drag_threshold;
        const bool y_check = fabsf(right_drag_delta.y) < drag_threshold;
        const bool right_click_no_drag = mouse_released && x_check && y_check;
        const bool wnd_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
        const bool editor_hovered = ImNodes::IsEditorHovered();

        const bool open_popup = wnd_focused && editor_hovered && right_click_no_drag;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
        if (!ImGui::IsAnyItemHovered() && open_popup)
        {
            ImGui::OpenPopup("add node");
        }

        if (ImGui::BeginPopup("add node"))
        {
            const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

            for (auto& effect_ref : effect_manager::effects()) {
                if (effect_ref.get().is_internal_node())
                    continue;
                if (ImGui::MenuItem(effect_ref.get().get_name())) {
                    add_node(effect_ref.get(), click_pos);
                }
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }

    for (auto& node : ui_nodes_) {
        ImNodes::BeginNode(node.node_id);

        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.fx->get_name());
        ImNodes::EndNodeTitleBar();

        const auto in_pins = node.fx->inputs();
        for (size_t i = 0; i < node.input_attr_ids.size(); ++i) {
            ImNodes::BeginInputAttribute(node.input_attr_ids[i]);
            ImGui::TextUnformatted(in_pins[i].name);
            ImNodes::EndInputAttribute();
        }

        ImGui::PushItemWidth(kNodeContentWidth);
        node.fx->run_ui();
        ImGui::PopItemWidth();

        const auto out_pins = node.fx->outputs();
        for (size_t i = 0; i < node.output_attr_ids.size(); ++i) {
            ImNodes::BeginOutputAttribute(node.output_attr_ids[i]);
            const float label_width = ImGui::CalcTextSize(out_pins[i].name).x;
            const float offset = kNodeContentWidth - label_width;
            if (offset > 0.0f) ImGui::Indent(offset);
            ImGui::TextUnformatted(out_pins[i].name);
            if (offset > 0.0f) ImGui::Unindent(offset);
            ImNodes::EndOutputAttribute();
        }

        bool shows_image_info = false;
        for (const auto& p : in_pins)  if (p.type == pin_type::image) shows_image_info = true;
        for (const auto& p : out_pins) if (p.type == pin_type::image) shows_image_info = true;
        if (shows_image_info) {
            ImGui::Spacing();
            ImGui::TextDisabled("%s", effect::describe_image(node.fx->get_preview_image()).c_str());
        }

        ImNodes::EndNode();
    }

    for (const auto& edge : graph_.edges())
    {
        ui_node* from_node = nullptr;
        ui_node* to_node = nullptr;
        if (find_ui_node(edge.from, &from_node) && find_ui_node(edge.to, &to_node))
        {
            ImNodes::Link(edge.id,
                from_node->output_attr_ids[edge.from_pin],
                to_node->input_attr_ids[edge.to_pin]);
        }
    }

    ImNodes::MiniMap(0.2f, minimap_location_);
    ImNodes::EndNodeEditor();

    {
        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {
            attr_info a, b;
            if (resolve_attr(start_attr, a) && resolve_attr(end_attr, b))
            {
                if (a.is_input) std::swap(a, b);

                const bool valid_link = !a.is_input && b.is_input && a.node_id != b.node_id && a.type == b.type;
                if (valid_link)
                {
                    std::vector<int> to_remove;
                    for (const auto& e : graph_.edges())
                        if (e.to == b.node_id && e.to_pin == b.pin_index)
                            to_remove.push_back(e.id);
                    for (int id : to_remove)
                        graph_.erase_edge(id);

                    graph_.insert_edge(a.node_id, b.node_id, a.pin_index, b.pin_index);
                }
            }
        }
    }

    {
        int link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            graph_.erase_edge(link_id);
        }
    }

    {
        int hovered_link_id;
        if (ImNodes::IsLinkHovered(&hovered_link_id) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            graph_.erase_edge(hovered_link_id);
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
                graph_.erase_edge(edge_id);
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedNodes();
        if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
        {
            selected_nodes_.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedNodes(selected_nodes_.data());
            for (const int node_id : selected_nodes_)
                remove_node(node_id);

            ImNodes::ClearNodeSelection();
        }
    }

    evaluate_and_show_output();
}

std::vector<pin_value> node_editor::evaluate_node(int node_id, std::unordered_map<int, std::vector<pin_value>>& cache)
{
    if (auto it = cache.find(node_id); it != cache.end())
        return it->second;

    ui_node* node_ptr = nullptr;
    if (!find_ui_node(node_id, &node_ptr))
        return {};

    effect& fx = *node_ptr->fx;
    const auto in_pins = fx.inputs();

    std::vector<pin_value> inputs(in_pins.size());
    for (size_t i = 0; i < in_pins.size(); ++i)
        inputs[i].type = in_pins[i].type;

    for (const auto& edge : graph_.edges())
    {
        if (edge.to == node_id && edge.to_pin < static_cast<int>(inputs.size()))
        {
            auto upstream = evaluate_node(edge.from, cache);
            if (edge.from_pin < static_cast<int>(upstream.size()))
            {
                inputs[edge.to_pin] = upstream[edge.from_pin];
                inputs[edge.to_pin].connected = true;
            }
        }
    }

    std::vector<pin_value> result = fx.run(inputs);
    cache[node_id] = result;
    return result;
}

void node_editor::update_downstream_ranges()
{
    for (auto& node : ui_nodes_)
    {
        const auto out_pins = node.fx->outputs();
        if (out_pins.size() != 1)
            continue;
        if (out_pins[0].type != pin_type::float_value && out_pins[0].type != pin_type::int_value)
            continue;

        bool  has_range = false;
        float range_min = 0.0f, range_max = 0.0f;

        for (const auto& edge : graph_.edges())
        {
            if (edge.from != node.node_id || edge.from_pin != 0)
                continue;

            ui_node* target = nullptr;
            if (!find_ui_node(edge.to, &target))
                continue;

            const auto target_in_pins = target->fx->inputs();
            if (edge.to_pin >= static_cast<int>(target_in_pins.size()))
                continue;

            const auto& pin = target_in_pins[edge.to_pin];
            if (pin.has_range)
            {
                has_range = true;
                range_min = pin.min;
                range_max = pin.max;
                break;
            }
        }

        node.fx->set_downstream_range(has_range, range_min, range_max);
    }
}

void node_editor::evaluate_and_show_output()
{
    last_output_ = cv::Mat();

    ui_node* input_node = nullptr;
    if (input_node_id_ != -1 && find_ui_node(input_node_id_, &input_node))
        input_node->fx->set_external_image(input_image_);

    std::unordered_map<int, std::vector<pin_value>> cache;

    for (const auto& node : ui_nodes_)
        evaluate_node(node.node_id, cache);

    if (output_node_id_ != -1)
    {
        ui_node* output_node = nullptr;
        if (find_ui_node(output_node_id_, &output_node))
            last_output_ = output_node->fx->get_preview_image();
    }
}

void node_editor::set_input_image(cv::Mat image)
{
    input_image_ = std::move(image);
}

cv::Mat node_editor::get_output() const
{
    return last_output_;
}

void node_editor::set_minimap_location(ImNodesMiniMapLocation location) {
    this->minimap_location_ = location;
}

ImNodesMiniMapLocation node_editor::get_minimap_location() const {
    return this->minimap_location_;
}