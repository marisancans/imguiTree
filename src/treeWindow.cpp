#include "treeWindow.h"
#include <math.h>
#include <sstream>
#include <vector>
#include "game.h"


static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }


void treeWindow(bool* opened, GameSettings& gameSettings){
    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin("Tree view", opened)) {
        ImGui::End();
        return;
    }

    int static currEditPlayer = 0;
    
    ImGui::BeginGroup();

    if (ImGui::Button("Computer vs computer"))
        game::init(gameSettings);


    ImGui::SameLine();
    if (ImGui::Button("Player vs computer"))
        game::init(gameSettings);


    ImGui::SameLine();
    ImGui::Checkbox("P1 starts?", &gameSettings.firstP1);

    ImGui::SameLine();
    ImGui::Checkbox("Hacks?", &gameSettings.hacks);
    ImGui::SameLine();
    ImGui::Checkbox("Debug?", &gameSettings.debug);
    ImGui::SameLine();
    ImGui::Checkbox("Trails?", &gameSettings.trails);
    

//    ImGui::InputText(
//

    // Scrolling
    ImGui::SliderInt("slider int2",  &gameSettings.levelOffsetX, 0, 255);
    ImGui::SliderInt("slider int",  &gameSettings.levelOffsetY, 0, 255);
    ImGui::SliderInt("Speed",  &gameSettings.speedMS, 1, 5000);

    if(gameSettings.debug) {
        ImGui::Text("Edit name:");
        ImGui::InputInt("##edit", &gameSettings.turnTimes);
        ImGui::RadioButton("P1", &currEditPlayer, 0);
        ImGui::RadioButton("P2", &currEditPlayer, 1);

        for (int r = 0; r < MRR; ++r) {
            for (int i = 0; i < 8; ++i) {
//            Cantor pairing function:
                int hash = ((i + r) * (i + r + 1)) / 2 + r;
                ImGui::PushID(hash);
                ImGui::Checkbox("", &gameSettings.movRange[currEditPlayer][i][r]);
                ImGui::SameLine();
                ImGui::PopID();
            }
            ImGui::NewLine();
        }
    }


    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
    
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);
    
    ImVec2 offset = ImGui::GetCursorScreenPos() + gameSettings.scrolling;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    
    if(ImGui::IsMouseHoveringWindow() && ImGui::IsMouseDragging())
        gameSettings.scrolling = gameSettings.scrolling + ImGui::GetIO().MouseDelta;

// ------------------  GRID -----------------------
    draw_list->ChannelsSplit(3);
    draw_list->ChannelsSetCurrent(1);
    
    const std::vector<NODE_VEC>& layers = game::nodes;
    
    int maxLayers = int(layers.size()) > 4 ? 3 : int(layers.size());
    for(int y = 0; y < maxLayers; ++y){
        int x = -int(layers[y].size())/2;
        for(auto& node : layers[y]){
            auto parentPos = ImVec2(x * gameSettings.levelOffsetX, y * gameSettings.levelOffsetY) + offset;
            
            draw_list->ChannelsSetCurrent(1);
    
    
            // Draw children
            for(auto &childID : node.childNodes) {
                int childXOffset = -int(layers[y+1].size())/2;
                auto childXPos = childID - layers[y+1][0].ID + childXOffset;
                auto childPos = ImVec2(childXPos * gameSettings.levelOffsetX, (y+1) *  gameSettings.levelOffsetY) + offset;
                ImU32 col = IM_COL32(100, 100, 100, 150);

                // If contains child and itself--- Dont do this, its ugly
                for(const auto& i : game::chosenPath)
                    if(childID == i.chldID && node.ID == i.parentID)
                        col = IM_COL32(0, 200, 0, 250);


                draw_list->AddLine(parentPos, childPos, col, 2.f);
            }
            
            
            //draw_list->AddCircleFilled(parentPos, 20, IM_COL32(100, 100, 100, 255));
            
            
            ImU32 col;
            switch(node.selected){
                case 1: col = IM_COL32(250, 0, 0, 150); break;
                case 2: col = IM_COL32(0, 0, 250, 150); break;
                default: col = IM_COL32(150, 250, 150, 150);
            }
            
            draw_list->AddCircleFilled(ImVec2(gameSettings.levelOffsetX * x,
                                              gameSettings.levelOffsetY * y) + offset,
                                       10, col);
            draw_list->ChannelsSetCurrent(2);
            
            

            if(gameSettings.debug) {
                std::ostringstream out;
                out << node.pos[P1].x << " | " << node.pos[P1].y << "\n" << node.pos[P2].x << " | " << node.pos[P2].y << "\n" << node.interspace;

                draw_list->AddText(ImVec2(gameSettings.levelOffsetX * x,
                                          gameSettings.levelOffsetY * y - 10) + offset,
                                   IM_COL32(250, 100, 100, 250), std::to_string(node.ID).c_str());

                draw_list->AddText(ImVec2(gameSettings.levelOffsetX * x,
                                          gameSettings.levelOffsetY * y) + offset,
                                   IM_COL32(250, 100, 100, 250), out.str().c_str());
            }
            
            x++;
        }
    }
    
    // Draw current player
    ImU32 col = game::_currPlayer == P1 ? IM_COL32(255, 0, 0, 150) : IM_COL32(0, 0, 255, 150);
    draw_list->AddCircleFilled(ImVec2(0, 0) + offset, 10, col);
    
    // Draw path taken
    for(int y = 0; y < maxLayers; ++y) {
        int x = -int(layers[y].size()) / 2;
        
    }
    
    
    
    ImU32 GRID_COLOR = IM_COL32(200, 0, 0,150);
    float GRID_SZ = 64.0f;
    
    
    draw_list->ChannelsSetCurrent(1);
    draw_list->ChannelsMerge();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::End();
}