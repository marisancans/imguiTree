#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>

class Node
{

public:
    Node(int id, std::string const& name, const ImVec2 &pos, int graphLevel,
         int health, int defense, int heal, int inputsCount, int outputsCount); //   input/output nodes are references to vector of node pointers

    ImVec2 getInputSlotPos() const;
    ImVec2 getOutputSlotPos() const;
    inline const char* getCName() { return name.c_str(); }
    inline void addOutput(Node* n) { outpuNodes.push_back(n); }
    inline void addOutput(std::vector<Node*>& nodes) { outpuNodes.insert(outpuNodes.end(), nodes.begin(), nodes.end()); }
    inline void setPos(ImVec2 pos){ this->pos = pos; }

    int                 id;
    std::string         name;
    ImVec2              pos;
    int                 graphLevel;
    ImVec2              size;
    int                 health;
    int                 defense;
    int                 heal;
    std::vector<Node*>  outpuNodes;
    int                 inputsCount;
    int                 outputsCount;
};


