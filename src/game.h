#pragma once

#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "board.h"


class Board;
class Node;
using NODE_VEC = std::vector<Node*>;

struct GameSettings{
    int maxLayer;
    int maxBoardX;
    int maxBoardY;
    const Movement P1MovRange;
    const Movement P2MovRange;
    ImVec2 scrolling;
    bool showGrid;
    int levelOffsetY;
    int levelOffsetX;
    int speedMS;
};

class Game {
public:
    enum GameMode{PvsPC = 1, PCvsPC = 2};
    enum Turn{P1 = 1, P2 = 2};
    Game(GameMode mode, Turn turn, GameSettings* gameSettings);
    void genLayer();
    inline const std::vector<NODE_VEC>& getLayers() const { return _nodes; }
    inline void swapTurn(){ _turn = _turn == P1 ? P2 : P1; }
    Node* createChild(Node* parent);
    Node* attack(Node* parent);
    Node* defend(Node* parent);
    Node* heal(Node* parent);
    GameSettings* gameSettings;

private:
    void init();
    int getNodeID() { return ++_nodeCount;}

    GameMode _mode;
    Turn _turn;
    Board* _board;
    int _nodeCount;
    std::vector<NODE_VEC> _nodes;
};
