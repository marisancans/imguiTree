#pragma once

#ifndef IMGUITEST_NODE_H
#define IMGUITEST_NODE_H

#include <string>
#include "imgui/imgui.h"

class Node
{

public:
    Node(int id, std::string name, const ImVec2& pos, float value,
         const ImVec4& color, int inputsCount, int outputsCount);

    ImVec2 GetInputSlotPos(int slot_no) const;
    ImVec2 GetOutputSlotPos(int slot_no) const;
    inline const char* getCName() { return name.c_str(); }

    int         id;
    std::string name;
    ImVec2      pos;
    ImVec2      size;
    float       value;
    ImVec4      color;
    int         inputsCount;
    int         outputsCount;
};

class NodeLink
{
public:
    inline NodeLink(int inputIdx, int inputSlot, int outputIdx, int outputSlot) :
            inputIdx(inputIdx), inputSlot(inputSlot), outputIdx(outputIdx), outputSlot(outputSlot)
    {

    };

    int         inputIdx;
    int         inputSlot;
    int         outputIdx;
    int         outputSlot;
};


#endif //IMGUITEST_NODE_H
