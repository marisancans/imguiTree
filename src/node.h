#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>
#include "board.h"

struct Position {
    int xPos;
    int yPos;
};



class Node
{
public:
    using NODE_VEC = std::vector<Node*>;
    enum Status{ROOT = 1, MIDDLE = 2, END = 3};
    inline Node(int id, Status status) : _id(id), _status(status) {};
    inline Node(Status status) : _status(status) {};
    Node(Node const* parent);
    ~Node() = default;

    void setHighlighted(bool b);

    NODE_VEC    parentNodes;
    NODE_VEC    childNodes;
    Position       P1State;
    Position       P2State;
    int         interspace; //Distance P1 <---> P2
    bool        selected;

private:
    int        _id;
    Status     _status;
};


