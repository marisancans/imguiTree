// Creating a node graph editor for ImGui
// Quick demo, not production code! This is more of a demo of how to use ImGui to create custom stuff.
// Better version by @daniel_collin here https://gist.github.com/emoon/b8ff4b4ce4f1b43e79f2
// See https://github.com/ocornut/imgui/issues/306
// v0.03: fixed grid offset issue, inverted sign of 'scrolling'
// Animated gif: https://cloud.githubusercontent.com/assets/8225057/9472357/c0263c04-4b4c-11e5-9fdf-2cd4f33f6582.gif

#include <math.h> // fmodf
#include "imgui/imgui.h"
#include <vector>
#include "node.h"
#include "layer.h"
#include "game.h"

// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

// Really dumb data structure provided for the example.
// Note that we storing links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
void ShowExampleAppCustomNodeGraph(bool* opened, Game const& game, GameSettings& gameSettings)
{
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Example: Custom Node Graph", opened))
    {
        ImGui::End();
        return;
    }

    static bool graphInited = false;
    static int nodeSelected = -1;



    // Draw a list of nodes on the left side
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    ImGui::BeginChild("node_list", ImVec2(100, 0));
    ImGui::Text("Nodes");
    ImGui::Separator();

    for (auto& l : game.getLayers()) {
        for(auto& n : l->getNodes()) {
            ImGui::PushID(n->id);
            if (ImGui::Selectable("tmp", n->id == nodeSelected))
                nodeSelected = n->id;
            if (ImGui::IsItemHovered()) {
                node_hovered_in_list = n->id;
                open_context_menu |= ImGui::IsMouseClicked(1);
            }
            ImGui::PopID();
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);


    // Create our child canvas
    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", gameSettings.scrolling.x, gameSettings.scrolling.y);
    ImGui::SliderInt("slider int2",  &gameSettings.levelOffsetXTo, 0, 255);
    ImGui::SliderInt("slider int",  &gameSettings.levelOffsetYTo, 0, 255);
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);

    ImGui::Checkbox("Show grid", &gameSettings.showGrid);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() + gameSettings.scrolling;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();


    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();

    // Display grid
    if (gameSettings.showGrid)
    {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f;
        for (float x = fmodf(gameSettings.scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
            draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
        for (float y = fmodf(gameSettings.scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
            draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
    }
//
//    // Display links
    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);                    // Background
    for (auto& l : game.getLayers()){               // Iterate through vector of nodes
        for(auto& curr : l->getNodes()){
            for (auto &in : curr->parentNodes) {    // Iterate through each nodes inputs and link them
                ImVec2 p1 = offset + in->getOutputSlotPos();
                ImVec2 p2 = offset + curr->getInputSlotPos();
                ImU32 color = in->isSelected(curr) ? IM_COL32(200, 200, 200, 255) : IM_COL32(200, 200, 100, 55);
                draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2,
                                          color, 3.0f);
            }
        }
    }

//
    // Display nodes
    // Iterate trough each level
    for(auto& l : game.getLayers()) {
        int nthNode = 0;
        for (auto &n : l->getNodes()) {
            nthNode++;
            ImGui::PushID(n->id);
            ImVec2 node_rect_min = offset + n->pos;
            // Display node contents first
            draw_list->ChannelsSetCurrent(2); // Foreground
            bool old_any_active = ImGui::IsAnyItemActive();
            ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
            ImGui::BeginGroup(); // Lock horizontal position
            ImGui::Text("%i", n->P1Stats.health);
            ImGui::SameLine(30);
            ImGui::Text("%i", n->P2Stats.health);
            ImGui::EndGroup();

            // Save the size of what we have emitted and whether any of the widgets are being used
            bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
            n->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
            ImVec2 node_rect_max = node_rect_min + n->size;


            // Adjust to gameSettings offsets
            int x = gameSettings.levelOffsetXTo;
            int y = gameSettings.levelOffsetYTo;
            n->setPos(nthNode, l->getLevel(), x, y);



            // Display node box
            draw_list->ChannelsSetCurrent(1); // Background
            ImGui::SetCursorScreenPos(node_rect_min);
            ImGui::InvisibleButton("node", n->size);
            if (ImGui::IsItemHovered()) {
                node_hovered_in_scene = n->id;
                open_context_menu |= ImGui::IsMouseClicked(1);
                n->setSelected(true);
            } else {
                n->setSelected(false);
            }
            bool node_moving_active = ImGui::IsItemActive();
            if (node_widgets_active || node_moving_active)
                nodeSelected = n->id;
            if (node_moving_active && ImGui::IsMouseDragging(0))
                n->pos = n->pos + ImGui::GetIO().MouseDelta;


            ImU32 node_bg_color = (node_hovered_in_list == n->id || node_hovered_in_scene == n->id ||
                                   (node_hovered_in_list == -1 && nodeSelected == n->id)) ? IM_COL32(75, 75, 75, 255)
                                                                                          : IM_COL32(60, 60, 60, 255);
            draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
            draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
            //        for (int slot_idx = 0; slot_idx < n->parentCount; slot_idx++)
            //            draw_list->AddCircleFilled(offset + n->getInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
            //        for (int slot_idx = 0; slot_idx < n->childrenCount; slot_idx++)
            //            draw_list->AddCircleFilled(offset + n->getOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));

            ImGui::PopID();
        }

        // Draw levels
        draw_list->ChannelsSetCurrent(0);
        ImU32 levelColor = l->getLevel() % 2 == 0 ? IM_COL32(20, 20, 20, 20) : IM_COL32(255, 255, 255, 20);
        draw_list->AddRectFilled(ImVec2(0, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo)),
                                 ImVec2(canvas_sz.x + 500,
                                        offset.y + ((l->getLevel() + 1) * gameSettings.levelOffsetYTo)),
                                 levelColor, 0.f, 0);

    }
    draw_list->ChannelsMerge();





    // Open context menu
    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
    {
        nodeSelected = node_hovered_in_list = node_hovered_in_scene = -1;
        open_context_menu = true;
    }
    if (open_context_menu)
    {
        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            nodeSelected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            nodeSelected = node_hovered_in_scene;
    }

//    // Draw context menu
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
//    if (ImGui::BeginPopup("context_menu"))
//    {
//        Node* node = nodeSelected != -1 ? &nodes[nodeSelected] : NULL;
//        ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
//        if (node)
//        {
//            ImGui::Text("Node '%s'", node->Name);
//            ImGui::Separator();
//            if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
//            if (ImGui::MenuItem("Delete", NULL, false, false)) {}
//            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
//        }
//        else
//        {
//            if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.Size, "New node", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2)); }
//            if (ImGui::MenuItem("Paste", NULL, false, false)) {}
//        }
//        ImGui::EndPopup();
//    }
//    ImGui::PopStyleVar();

    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
        gameSettings.scrolling = gameSettings.scrolling + ImGui::GetIO().MouseDelta;

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();

    ImGui::End();

    graphInited = true;
}