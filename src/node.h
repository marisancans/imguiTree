#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>

class Node
{

public:
    Node(int id, std::string const& name, const ImVec2 &pos, int graphLevel,
         int health, int attack, int defense, int heal, int inputsCount, int outputsCount); //   input/output nodes are references to vector of node pointers
    Node(const Node& parent, int id, std::string name);

    ImVec2 getInputSlotPos() const;
    ImVec2 getOutputSlotPos() const;
    inline const char* getCName() { return name.c_str(); }
    inline void addInput(Node* n) { inputNodes.push_back(n); inputsCount++; }
    inline void addInput(std::vector<Node*>& nodes) { inputNodes.insert(inputNodes.end(), nodes.begin(), nodes.end()); }
    inline void setPos(ImVec2 pos){ this->pos = pos; }
    inline bool isAlive(){ return bool(health); }

    int                 id;
    std::string         name;
    ImVec2              pos;
    int                 graphLevel;
    ImVec2              size;
    int                 health;
    int                 attack;
    int                 defense;
    int                 heal;
    std::vector<Node*>  inputNodes;
    int                 inputsCount;
    int                 outputsCount;
};


