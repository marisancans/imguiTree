#include "treeWindow.h"
#include <math.h>
#include <sstream>
#include <vector>


static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }


void treeWindow(bool* opened, Game& game, GameSettings& gameSettings){
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Tree view", opened)) {
        ImGui::End();
        return;
    }


    ImGui::BeginGroup();

    ImGui::SliderInt("slider int2",  &gameSettings.levelOffsetX, 0, 255);
    ImGui::SliderInt("slider int",  &gameSettings.levelOffsetY, 0, 255);
    ImGui::SliderInt("Speed",  &gameSettings.speedMS, 1, 5000);
    if (ImGui::Button("Next generation"))
        game.makeTurns();


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


    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);                    // Background



    int y = 0;
    const std::vector<NODE_VEC>& layers = game.getLayers();

    int maxLayers = int(game.getLayers().size()) > 4 ? 3 : int(game.getLayers().size());
    for(int cLayer = 0; cLayer < maxLayers; ++cLayer){
        int x = 1;

        for(auto& node : layers[cLayer]){
            draw_list->ChannelsSetCurrent(1);
            if(!node.childNodes.empty()) {
                for (auto &childID : node.childNodes) {
                    draw_list->AddLine(ImVec2(x * gameSettings.levelOffsetX, y * gameSettings.levelOffsetY) + win_pos + offset,
                                       ImVec2(childID * gameSettings.levelOffsetX, (y+1) *  gameSettings.levelOffsetY) + win_pos + offset,
                                       IM_COL32(100, 100, 100, 150), 2.f);
                    draw_list->AddCircleFilled(ImVec2(x * gameSettings.levelOffsetX, y * gameSettings.levelOffsetY)
                    + win_pos + offset, 35, IM_COL32(100, 100, 100, 255));
                }
            }

            ImU32 col;
            switch(node.selected){
                case 1: col = IM_COL32(250, 0, 0, 150); break;
                case 2: col = IM_COL32(0, 0, 250, 150); break;
                default: col = IM_COL32(150, 250, 150, 150);
                }

            draw_list->AddCircleFilled(ImVec2(gameSettings.levelOffsetX * x,
                                                  gameSettings.levelOffsetY * y) + win_pos + offset,
                                           10, col);
            draw_list->ChannelsSetCurrent(2);


            std::ostringstream out;
            out << node.P1Pos.x << " | " << node.P1Pos.y << "\n" << node.P2Pos.x << " | " << node.P2Pos.y << "\n" << node.interspace;

            draw_list->AddText(ImVec2(gameSettings.levelOffsetX * x,
                                                  gameSettings.levelOffsetY * y) + win_pos + offset,
                                   IM_COL32(250, 100, 100, 250), out.str().c_str());

            x++;
        }

        y++;
    }
//    draw_list->AddLine(ImVec2(x * spacingX, 1) + win_pos, ImVec2(x * spacingX, canvas_sz.y) + win_pos, GRID_COLOR);
    ImU32 GRID_COLOR = IM_COL32(200, 0, 0,150);

    float GRID_SZ = 64.0f;

//    for (float x = fmodf(gameSettings.scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
//        draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
//    for (float y = fmodf(gameSettings.scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
//        draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);



    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
        gameSettings.scrolling = gameSettings.scrolling + ImGui::GetIO().MouseDelta;



    draw_list->ChannelsSetCurrent(1);



    draw_list->ChannelsMerge();


    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
    ImGui::EndGroup();

    ImGui::End();
}