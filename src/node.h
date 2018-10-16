#pragma once

#include <string>
#include "imgui/imgui.h"
//#include "game.h"
#include <vector>
#include "board.h"

struct State {
    int     xPos;
    int     yPos;
//    Movement potMove; // potential movement
};



class Node
{
public:
    using NODE_VEC = std::vector<Node*>;
    enum Status{ROOT = 1, MIDDLE = 2, END = 3};
    inline Node(int id, Status status) : _id(id), _status(status) {};
    Node(int id, Node& parent);
    ~Node() = default;

    void setHighlighted(bool b);

    NODE_VEC    parentNodes;
    NODE_VEC    childNodes;
    State       P1State;
    State       P2State;
    bool        selected;

private:
    int        _id;
    Status     _status;
};


