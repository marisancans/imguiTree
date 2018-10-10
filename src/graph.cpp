//
// Created by maris on 10/7/18.
//

#include "graph.h"
#include "node.h"

Graph::Graph(bool showGrid, std::vector<Layer*> const& layers) : showGrid(showGrid),  layers(layers){
    scrolling = ImVec2(0.0f, 0.0f);
    levelOffsetYTo = 150;
    levelOffsetXTo = 150;
}

