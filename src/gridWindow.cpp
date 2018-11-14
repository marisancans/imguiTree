#include <math.h> // fmodf
#include "imgui/imgui.h"
#include <vector>
#include "node.h"
#include "game.h"
#include "iostream"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }


void gridWindow(bool* opened, GameSettings& gameSettings) {
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Example: Custom Node Graph", opened)) {
        ImGui::End();
        return;
    }
    
    game::makeTurns(gameSettings);
    
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





    
    
    ImGui::BeginGroup();
    
    
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
    
    ImGui::BeginChild("scroll grid", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();

// ------------------  GRID -----------------------
    
    ImU32 GRID_COLOR = IM_COL32(100, 100, 100, 50);
    float spacingX = canvas_sz.x / gameSettings.maxBoardX;
    float spacingY = canvas_sz.y / gameSettings.maxBoardY;
    
    
    // Drawing grid
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
    lDrawCurr(game::currPos[P1], IM_COL32(200, 0, 0, 150));
    lDrawCurr(game::currPos[P2], IM_COL32(0, 0, 200, 150));
    
    
    // FOOLING AROUND HERE

    //    and ranges

    POS_VEC ranges = game::getRanges();

    for(const auto& p : ranges)
        lDrawPos(p, IM_COL32(0, 150, 0, 50));

    if(gameSettings.trails) {
        for (int i = 0; i < game::tracers[P1].size(); ++i)
            lDrawCurr(game::tracers[P1][i], IM_COL32(20 * i, 0, 0, 20 * i));
        for (int i = 0; i < game::tracers[P2].size(); ++i)
            lDrawCurr(game::tracers[P2][i], IM_COL32(0, 0, 20 * i, 20 * i));
    }

    if(gameSettings.debug)
        draw_list->AddLine(ImVec2(game::currPos[P1].x * spacingX + spacingX/2, game::currPos[P1].y * spacingY + spacingY/2) + win_pos,
                           ImVec2(game::currPos[P2].x * spacingX + spacingX/2, game::currPos[P2].y * spacingY + spacingY/2) + win_pos, IM_COL32(0, 150, 0 ,250));


    // Clicking handler
    ImGuiIO& io = ImGui::GetIO();

    io.WantCaptureMouse = true;

//    if (ImGui::IsMouseDown(0)) {
        auto sp = ImGui::GetIO().MousePos;
        float xgrid = (sp.x - win_pos.x) / gameSettings.maxBoardX;
        float xcircle = xgrid * (spacingX/2);
        float ygrid = (sp.y - win_pos.y) / gameSettings.maxBoardY;
        float ycircle = ygrid * (spacingY/2);
        draw_list->AddCircleFilled({xcircle + win_pos.x, ycircle + win_pos.y},
                                   std::min(spacingX / 2, spacingY / 2), IM_COL32(244, 110, 65, 155));
//        ImVec2(canvas_sz.x, y * spacingY)
        if(gameSettings.debug)
            ImGui::Text("x: '%f | y: '%f' | fps: %f",  xgrid, ygrid, io.Framerate);
//    }

    if(ImGui::IsMouseDown(0)) {
        game::lastClicked = Position({int(xgrid / 2), int(ygrid / 2)});
        game::humanClicked = true;
    }

    auto width = ImGui::GetWindowWidth();
    ImGui::SetWindowSize({100, 100});


    ImGui::Text("Turn times: %i | %i", game::tt, gameSettings.turnTimes);


    if(game::won)
        ImGui::Text("Player %i won!", game::winnerID);




//
    //    // Display links
    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);                    // Background
    
    
    draw_list->ChannelsSetCurrent(2); // Foreground
    bool old_any_active = ImGui::IsAnyItemActive();
    
    ImGui::BeginGroup(); // Lock horizontal position
    
    ImGui::EndGroup();
    
    // Save the size of what we have emitted and whether any of the widgets are being used
    bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
    
    // Display node box
    draw_list->ChannelsSetCurrent(1); // Background
    
    if(ImGui::IsItemClicked(0)) {
//            nodeClicked = n;
//            n->setHighlighted(true);
    }
    
    draw_list->ChannelsSetCurrent(1);
    draw_list->ChannelsMerge();
    
    
    // Draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    if (ImGui::BeginPopup("context_menu"))
    {
//        Node* node = nodeSelected != -1 ? &nodes[nodeSelected] : NULL;
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
    
    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();
    
    ImGui::End();
    
    graphInited = true;
}