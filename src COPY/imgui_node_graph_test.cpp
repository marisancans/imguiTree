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
#include "iostream"

// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

// Really dumb data structure provided for the example.
// Note that we storing links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
void ShowExampleAppCustomNodeGraph(bool* opened, Game const& game, GameSettings& gameSettings) {
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Example: Custom Node Graph", opened)) {
        ImGui::End();
        return;
    }

    static bool graphInited = false;
    static int nodeSelected = -1;
    static Node *nodeClicked = nullptr;

    // Draw a list of nodes on the left side
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;


    const float NODE_SLOT_RADIUS = 4.0f;
    const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);
//
//


    ImGui::BeginGroup();


    ImGui::BeginChild("Child1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.35f, 60), false);

    if (nodeClicked != nullptr) {

        ImGui::Columns(6, "mycolumns");//TODO remove id from colum, a it is a duplicate
        ImGui::Separator();
        ImGui::Text("Player");
        ImGui::NextColumn();
        ImGui::Text("Move");
        ImGui::NextColumn();
        ImGui::Text("Health");
        ImGui::NextColumn();
        ImGui::Text("Attack");
        ImGui::NextColumn();
        ImGui::Text("Defense");
        ImGui::NextColumn();
        ImGui::Text("Heal");
        ImGui::NextColumn();
        ImGui::Separator();

        std::vector<PlayerStats*> stats{&nodeClicked->P1Stats, &nodeClicked->P2Stats};

        bool p = false;
        for(auto& s : stats) {
            std::string turn = ! p ? "P1" : "P2";
            ImGui::Text(turn.c_str());
            p = true;
            ImGui::NextColumn();
            ImGui::Text(nodeClicked->getMoveStr());
            ImGui::NextColumn();
            ImGui::Text(std::to_string(s->health).c_str());
            ImGui::NextColumn();
            ImGui::Text(std::to_string(s->attackCount).c_str());
            ImGui::NextColumn();
            ImGui::Text(std::to_string(s->defenseCount).c_str());
            ImGui::NextColumn();
            ImGui::Text(std::to_string(s->healCount).c_str());
            ImGui::NextColumn();
        }
    }

    ImGui::EndChild();


    ImGui::SameLine();

    ImGui::BeginChild("Child2", ImVec2(0, 50), true);
    ImGui::Text("%04d: scrollable region", 2);
    ImGui::EndChild();

    ImGui::BeginChild("Child3", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 60));
    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", gameSettings.scrolling.x, gameSettings.scrolling.y);
    ImGui::SliderInt("slider int2",  &gameSettings.levelOffsetXTo, 0, 255);
    ImGui::SliderInt("slider int",  &gameSettings.levelOffsetYTo, 0, 255);
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::Checkbox("Show grid", &gameSettings.showGrid);

    ImGui::EndChild();

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

                ImU32 selectOffset = IM_COL32(0, 0, 0, 0);
                ImU32 color = IM_COL32(100, 100, 100, 100);

                if(in->isSelected(curr)) {
                    switch (curr->move) {
                        case Node::ATTACK: selectOffset = IM_COL32(150, 0, 0, 100); break;
                        case Node::DEFEND: selectOffset = IM_COL32(0, 0, 150, 100); break;
                        case Node::HEAL:   selectOffset = IM_COL32(0, 150, 0, 100); break;
                    }

                    switch (curr->move) {
                        case Node::ATTACK: color = IM_COL32(50, 0, 0, 100) + selectOffset; break;
                        case Node::DEFEND: color = IM_COL32(0, 0, 50, 100) + selectOffset; break;
                        case Node::HEAL:   color = IM_COL32(0, 50, 0, 100) + selectOffset; break;
                        default: color = IM_COL32(150, 150, 150, 100);
                    }
                }


                draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2,
                                          color, 3.0f);// TODO based on which node is futher, change berzier initial curve
            }
        }
    }

//
    // Display nodes
    // Iterate trough each level
    auto layers = game.getLayers();
    for(auto& l : layers) {
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
            ImGui::Text("%i |", n->P1Stats.health);
            ImGui::SameLine(20);
            ImGui::Text(" %i", n->P2Stats.health);
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
                n->setHighlighted(true);
                nodeClicked = n;
            } else {
                n->setHighlighted(false);
            }

            if(ImGui::IsItemClicked(0)) {
                nodeClicked = n;
                n->setHighlighted(true);
            }

            bool node_moving_active = ImGui::IsItemActive();
//            if (node_widgets_active || node_moving_active)
//                nodeSelected = n->id;
            if (node_moving_active && ImGui::IsMouseDragging(0))
                n->pos = n->pos + ImGui::GetIO().MouseDelta;


            //Show dead node
            int colorOffset = 0;
            if(n->getStatus() == Node::END)
                if(n->getNextTurnStats(l->getCurrentTurn())->health <= 0)
                    colorOffset = 50;



            ImU32 node_bg_color = (node_hovered_in_list == n->id || node_hovered_in_scene == n->id ||
                                   (node_hovered_in_list == -1 && n->selected)) ? IM_COL32(75 + colorOffset , 75, 75, 255)
                                                                                          : IM_COL32(60 + colorOffset, 60, 60, 255);
            draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
            draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
            //        for (int slot_idx = 0; slot_idx < n->parentCount; slot_idx++)
            //            draw_list->AddCircleFilled(offset + n->getInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
            //        for (int slot_idx = 0; slot_idx < n->childrenCount; slot_idx++)
            //            draw_list->AddCircleFilled(offset + n->getOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));

            ImGui::PopID();
        }

        // Draw levels
        draw_list->ChannelsSetCurrent(1);
//        draw_list->AddRect(, ImVec2(100,
//                                        offset.y + ((l->getLevel() + 1) * gameSettings.levelOffsetYTo)), , 0, 0, 5);
//        draw_list->AddCircle(ImVec2(25, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo) + gameSettings.levelOffsetYTo/2),
//                20, IM_COL32(100, 100, 100, 255), 12, 5.0f);

        draw_list->AddText(ImVec2(25, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo) + gameSettings.levelOffsetYTo/2),
                           IM_COL32(50, 250, 50, 255), l->getTurnStr().c_str());

        draw_list->ChannelsSetCurrent(0);
        ImU32 levelColor = l->getLevel() % 2 == 0 ? IM_COL32(20, 20, 20, 20) : IM_COL32(255, 255, 255, 20);
        draw_list->AddRectFilled(ImVec2(0, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo)),
                                 ImVec2(canvas_sz.x + 500,
                                        offset.y + ((l->getLevel() + 1) * gameSettings.levelOffsetYTo)),
                                 levelColor, 0.f, 0);

    }
    draw_list->ChannelsMerge();





//    // Open context menu
//    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
//    {
//        nodeSelected = node_hovered_in_list = node_hovered_in_scene = -1;
//        open_context_menu = true;
//    }
//    if (open_context_menu)
//    {
//        ImGui::OpenPopup("context_menu");
//        if (node_hovered_in_list != -1)
//            nodeSelected = node_hovered_in_list;
//        if (node_hovered_in_scene != -1)
//            nodeSelected = node_hovered_in_scene;
//    }

    // Draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("context_menu"))
    {
//        Node* node = nodeSelected != -1 ? &nodes[nodeSelected] : NULL;
        ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
        if (true)
        {
//            ImGui::Text("Node '%s'", node->Name);
            ImGui::Separator();
            if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
            if (ImGui::MenuItem("Delete", NULL, false, false)) {}
            if (ImGui::MenuItem("Copy", NULL, false, false)) {}
        }
        else
        {
//            if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.Size, "New node", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2)); }
            if (ImGui::MenuItem("Paste", NULL, false, false)) {}
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();

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