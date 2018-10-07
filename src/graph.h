#pragma once

#include "imgui/imgui.h"
#include <vector>
class Node;
class Graph {
public:
    Graph(bool showGrid);
    inline void addNode(Node* n){ nodes.push_back(n);}
    inline void addNode(std::vector<Node*>& nodes) {
        this->nodes.insert(this->nodes.end(), nodes.begin(), nodes.end());
    }

    std::vector<Node*> nodes;
    ImVec2 scrolling;
    bool showGrid;
};
