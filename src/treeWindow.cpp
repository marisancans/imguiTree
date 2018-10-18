#include "treeWindow.h"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }


void treeWindow(bool* opened, Game const& game, GameSettings& gameSettings){
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Tree view", opened)) {
        ImGui::End();
        return;
    }


    ImGui::BeginGroup();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));

    ImDrawList* draw_list = ImGui::GetWindowDrawList();


    ImVec2 win_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_sz = ImGui::GetWindowSize();

// ------------------  GRID -----------------------

    ImU32 GRID_COLOR = IM_COL32(200, 0, 0,150);
    float spacingX = canvas_sz.x;
    float spacingY = canvas_sz.y;


    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);                    // Background

    ImGui::BeginGroup(); // Lock horizontal position

    int y = 0;
    for(auto& layers : game.getLayers()){
        int x = 1;

        for(auto& node : layers){
                for(auto& parentID : node->parentNodes) {
                    draw_list->AddLine(ImVec2(gameSettings.levelOffsetX * (parentID + 1), gameSettings.levelOffsetY * (y - 1)) + win_pos,
                                       ImVec2(gameSettings.levelOffsetX * x, gameSettings.levelOffsetY * y) + win_pos,
                                       IM_COL32(200, 0, 0, 150));
                }

                draw_list->AddCircleFilled(ImVec2(gameSettings.levelOffsetX * x,
                                                  gameSettings.levelOffsetY * y) + win_pos,
                                           50, IM_COL32(150, 150, 150, 150));

            x++;
        }

        y++;
    }
//    draw_list->AddLine(ImVec2(x * spacingX, 1) + win_pos, ImVec2(x * spacingX, canvas_sz.y) + win_pos, GRID_COLOR);

    ImGui::EndGroup();

//    draw_list->AddCircleFilled(ImVec2(50 ,50), 100, IM_COL32(150, 150, 150, 150));

    draw_list->ChannelsSetCurrent(1);



    draw_list->ChannelsMerge();


    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();

    ImGui::End();
}