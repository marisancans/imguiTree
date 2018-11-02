#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "strategies.h"

struct Position {
    int x;
    int y;
    
    bool operator ==(const Position & obj) const
    {
        return x == obj.x && y == obj.y;
    }
};

struct Node
{
    using NODE_VEC = std::vector<Node*>;
    enum Status{ROOT = 1, MIDDLE = 2, END = 3};
    inline Node():_status(ROOT), ID(0), selected(0){};
    
    int              parentNodeID = 0;
    std::vector<int> childNodes;
    Position         pos[PLAYER_COUNT] = {};
    double           interspace = 0; //Distance P1 <---> P2
    int              selected = 0;
    int        ID = 0;
    
    inline void calcInterspace()
    {
        interspace = std::abs(pos[P1].x - pos[P2].x) + std::abs(pos[P1].y - pos[P2].y);
    }
    Status     _status = ROOT;
};


