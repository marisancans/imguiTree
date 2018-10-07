#pragma once

#include "imgui/imgui.h"
#include <vector>
#include <map>

class Node;

using level = std::pair<int, std::vector<Node*>>;

class Node;
class Graph {
public:
    explicit Graph(bool showGrid);
    void addNode(Node* n);
    void addNode(std::vector<Node*>& nodes);

    std::map<int, std::vector<Node*>> layout;
    ImVec2 scrolling;
    bool showGrid;
};
