#pragma once

#include "imgui/imgui.h"
#include <vector>
#include <map>

class Node;

class Graph {
public:
    explicit Graph(bool showGrid, std::vector<Node*>& nodes);
    std::map<int, int> layout;
    std::vector<Node*>& _nodes;
    ImVec2 scrolling;
    bool showGrid;
};
