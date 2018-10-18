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
    inline Node(int id, Status status, Position P1StartPos, Position P2StartPos):
        _id(id), _status(status), P1Pos(P1StartPos), P2Pos(P2StartPos){};
    inline Node(Status status) : _status(status) {};
    Node(Node const* parent);
    ~Node() = default;

    void setHighlighted(bool b);

    NODE_VEC    parentNodes;
    NODE_VEC    childNodes;
    Position       P1Pos;
    Position       P2Pos;
    int         interspace; //Distance P1 <---> P2
    bool        selected;

private:
    int        _id;
    Status     _status;
};


