#include <math.h> // fmodf
#include "imgui/imgui.h"
#include <vector>
#include "node.h"
#include "game.h"
#include "iostream"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

void gridWindow(bool* opened, Game& game, GameSettings& gameSettings) {
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Example: Custom Node Graph", opened)) {
        ImGui::End();
        return;
    }

    game.makeTurns();

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
    bool won = false;
    ImGui::Begin("About Dear ImGui", &won, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Dear ImGui, %s", ImGui::GetVersion());
    ImGui::Separator();
    ImGui::Text("By Omar Cornut and all dear imgui contributors.");
    ImGui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");
    ImGui::End();



    ImGui::BeginGroup();



    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));

    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() + gameSettings.scrolling;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();


    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();

// ------------------  GRID -----------------------

    ImU32 GRID_COLOR = IM_COL32(200, 0, 0,150);
    float spacingX = canvas_sz.x / gameSettings.maxBoardX;
    float spacingY = canvas_sz.y / gameSettings.maxBoardY;


    for(int x = 0; x < gameSettings.maxBoardX; ++x){
        for(int y = 0; y < gameSettings.maxBoardY; ++y){
            draw_list->AddLine(ImVec2(x * spacingX, 1) + win_pos, ImVec2(x * spacingX, canvas_sz.y) + win_pos, GRID_COLOR);
            draw_list->AddLine(ImVec2(1, y * spacingY) + win_pos, ImVec2(canvas_sz.x, y * spacingY) + win_pos, GRID_COLOR);
        }
    }



    for(int x = 0; x < gameSettings.maxBoardX; ++x){
        for(int y = 0; y < gameSettings.maxBoardY; ++y) {
//            draw_list->AddRectFilled(ImVec2())
        }
    }


    auto lDrawPos = [&](const Position& pos, ImU32 col){
                                       draw_list->AddRectFilled(ImVec2(pos.x * spacingX, pos.y * spacingY) + win_pos,
                                       ImVec2((pos.x + 1 )* spacingX, (pos.y + 1) * spacingY) + win_pos,
                                       col);
                                      };

    auto lDrawCurr = [&](const Position& pos, ImU32 col){
        draw_list->AddCircleFilled(ImVec2(pos.x * spacingX + spacingX/2, pos.y * spacingY + spacingY/2) + win_pos,
                                   spacingX/2, col);
    };

//    Display current nodes
    lDrawCurr(game.currPosP1, IM_COL32(200, 0, 0, 150));
    lDrawCurr(game.currPosP2, IM_COL32(0, 0, 200, 150));


    // FOOLING AROUND HERE
    draw_list->AddLine(ImVec2(game.currPosP1.x * spacingX + spacingX/2, game.currPosP1.y * spacingY + spacingY/2) + win_pos,
                       ImVec2(game.currPosP2.x * spacingX + spacingX/2, game.currPosP2.y * spacingY + spacingY/2) + win_pos, IM_COL32(0, 150, 0 ,250));

    for(int i = 0; i < game.tracersP1.size(); ++i)
        lDrawCurr(game.tracersP1[i], IM_COL32(20 * i, 0, 0, 250));
    for(int i = 0; i < game.tracersP2.size(); ++i)
        lDrawCurr(game.tracersP2[i], IM_COL32(0, 0, 20 * i, 250));



    //    and ranges

    POS_VEC ranges = game.getRanges();

    for(const auto& p : ranges)
        lDrawPos(p, IM_COL32(0, 100, 0, 50));




//
//    // Display links
    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);                    // Background

//    draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2,
//                              color, 3.0f);// TODO based on which node is futher, change berzier initial curve




//        ImGui::PushID(n->id);
//        ImVec2 node_rect_min = offset + n->pos;
        // Display node contents first
        draw_list->ChannelsSetCurrent(2); // Foreground
        bool old_any_active = ImGui::IsAnyItemActive();
//        ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
        ImGui::BeginGroup(); // Lock horizontal position
//        ImGui::Text("%i |", n->P1Stats.health);
//        ImGui::SameLine(20);
//        ImGui::Text(" %i", n->P2Stats.health);
        ImGui::EndGroup();

        // Save the size of what we have emitted and whether any of the widgets are being used
        bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
//        n->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
//        ImVec2 node_rect_max = node_rect_min + n->size;


        // Adjust to gameSettings offsets
//        int x = gameSettings.levelOffsetX;
//        int y = gameSettings.levelOffsetY;
//        n->setPos(nthNode, l->getLevel(), x, y);



        // Display node box
        draw_list->ChannelsSetCurrent(1); // Background
//        ImGui::SetCursorScreenPos(node_rect_min);
//        ImGui::InvisibleButton("node", n->size);
//        if (ImGui::IsItemHovered()) {
//            node_hovered_in_scene = n->id;
//            n->setHighlighted(true);
//            nodeClicked = n;
//        } else {
//            n->setHighlighted(false);
//        }

        if(ImGui::IsItemClicked(0)) {
//            nodeClicked = n;
//            n->setHighlighted(true);
        }

        bool node_moving_active = ImGui::IsItemActive();
//            if (node_widgets_active || node_moving_active)
//                nodeSelected = n->id;
//        if (node_moving_active && ImGui::IsMouseDragging(0))






//        ImU32 node_bg_color = (node_hovered_in_list == n->id || node_hovered_in_scene == n->id ||
//                               (node_hovered_in_list == -1 && n->selected)) ? IM_COL32(75 + colorOffset , 75, 75, 255)
//                                                                                      : IM_COL32(60 + colorOffset, 60, 60, 255);
//        draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
//        draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
        //        for (int slot_idx = 0; slot_idx < n->parentCount; slot_idx++)
        //            draw_list->AddCircleFilled(offset + n->getInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
        //        for (int slot_idx = 0; slot_idx < n->childrenCount; slot_idx++)
        //            draw_list->AddCircleFilled(offset + n->getOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));

//        ImGui::PopID();


        // Draw levels
        draw_list->ChannelsSetCurrent(1);
//        draw_list->AddRect(, ImVec2(100,
//                                        offset.y + ((l->getLevel() + 1) * gameSettings.levelOffsetYTo)), , 0, 0, 5);
//        draw_list->AddCircle(ImVec2(25, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo) + gameSettings.levelOffsetYTo/2),
//                20, IM_COL32(100, 100, 100, 255), 12, 5.0f);

//        draw_list->AddText(ImVec2(25, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo) + gameSettings.levelOffsetYTo/2),
//                           IM_COL32(50, 250, 50, 255), l->getTurnStr().c_str());

//        draw_list->ChannelsSetCurrent(0);
//        ImU32 levelColor = l->getLevel() % 2 == 0 ? IM_COL32(20, 20, 20, 20) : IM_COL32(255, 255, 255, 20);
//        draw_list->AddRectFilled(ImVec2(0, offset.y + (l->getLevel() * gameSettings.levelOffsetYTo)),
//                                 ImVec2(canvas_sz.x + 500,
//                                        offset.y + ((l->getLevel() + 1) * gameSettings.levelOffsetYTo)),
//                                 levelColor, 0.f, 0);


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