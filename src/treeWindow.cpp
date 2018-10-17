#include "treeWindow.h"


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
    float spacingX = canvas_sz.x / gameSettings.maxBoardX;
    float spacingY = canvas_sz.y / gameSettings.maxBoardY;


    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);                    // Background

    ImGui::BeginGroup(); // Lock horizontal position

    ImGui::EndGroup();

    draw_list->AddCircleFilled(ImVec2(50 ,50), 100, IM_COL32(150, 150, 150, 150));

    draw_list->ChannelsSetCurrent(1);



    draw_list->ChannelsMerge();


    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();

    ImGui::End();
}