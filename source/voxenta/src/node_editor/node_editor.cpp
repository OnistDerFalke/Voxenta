#include "voxenta/node_editor/node_editor.h"

node_editor::node_editor(EditorContext *m_context) {
    this->m_context = m_context;
}

void node_editor::show(ImVec2 size) {
    SetCurrentEditor(m_context);
    Begin("Node editor", size);
    int uniqueId = 1;
    // Start drawing nodes.
    BeginNode(uniqueId++);
    ImGui::Text("Node A");
    BeginPin(uniqueId++,  PinKind::Input);
    ImGui::Text("-> In");
    EndPin();
    ImGui::SameLine();
    BeginPin(uniqueId++,  PinKind::Output);
    ImGui::Text("Out ->");
    EndPin();
    EndNode();
    ImGui::SameLine();
    ImGui::Text("Description:");
    ImGui::Dummy(ImVec2(0, 5));
    ImGui::TextWrapped("%s", "Load the image first to apply any effects.");
    End();
    SetCurrentEditor(nullptr);
}
