#ifndef VOXENTA_NODE_EDITOR_H
#define VOXENTA_NODE_EDITOR_H

#include <imgui_node_editor.h>

using namespace ax::NodeEditor;

class node_editor {
public:
    node_editor() = default;
    explicit node_editor(EditorContext* m_context);
    void show(ImVec2 size);
private:
    EditorContext* m_context;
};

#endif
