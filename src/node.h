#pragma once

#include <string>
#include "imgui/imgui.h"
#include "game.h"
#include "playerStats.h"
#include <vector>

struct State {
    int           xPos;
    int           yPos;
    MovementRange possMove; // Possible movement

//    inline bool operator==(PlayerStats const& parent){
//        return health == parent.health;
//    }
};

using NODE_VEC = std::vector<Node*>;

class Node
{
public:
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


