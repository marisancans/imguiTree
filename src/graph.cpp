//
// Created by maris on 10/7/18.
//

#include "graph.h"
#include "node.h"


Graph::Graph(bool showGrid) : showGrid(showGrid){
    scrolling = ImVec2(0.0f, 0.0f);
}

