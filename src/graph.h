#pragma once

#include "imgui/imgui.h"
#include <vector>
#include <map>

class Node;
class Layer;

class Graph {
public:
    Graph(bool showGrid, std::vector<Layer*> const& layers);
    ImVec2 scrolling;
    bool showGrid;
    std::vector<Layer*> layers;
};
