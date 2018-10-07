#include "node.h"


Node::Node(int id, std::string const& name, const ImVec2 &pos, int graphLevel,
          int health, int defense, int heal, int inputsCount, int outputsCount) :
           id(id), name(name), pos(pos), graphLevel(graphLevel), health(health),
           defense(defense), heal(heal), inputsCount(inputsCount), outputsCount(outputsCount)
    {

    }

ImVec2 Node::getInputSlotPos() const
{
    return ImVec2(pos.x + size.x  / ((float)inputsCount + 1), + pos.y);
}

ImVec2 Node::getOutputSlotPos() const
{
    return ImVec2(pos.x + size.x / ((float)outputsCount + 1), pos.y + size.y);
}




