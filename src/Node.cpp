#include "Node.h"


Node::Node(int id, std::string name, const ImVec2& pos, float value,
           const ImVec4& color, int inputsCount, int outputsCount) :
           id(id), name(name), pos(pos), value(value), color(color),
           inputsCount(inputsCount), outputsCount(outputsCount)
    {

    }

ImVec2 Node::GetInputSlotPos(int slot_no) const
{
    return ImVec2(pos.x + size.x * ((float)slot_no + 1) / ((float)inputsCount + 1), + pos.y);
}

ImVec2 Node::GetOutputSlotPos(int slot_no) const
{
    return ImVec2(pos.x + size.x * ((float)slot_no + 1) / ((float)outputsCount + 1), pos.y + size.y);
}



