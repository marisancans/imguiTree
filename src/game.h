#pragma once

#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "board.h"
#include "node.h"


class Node;
struct moveMatrix;
using NODE_VEC = std::vector<Node*>;


struct GameSettings{
    int maxLayer;
    int maxBoardX;
    int maxBoardY;
    const Board::moveMatrix P1MovRange;
    const Board::moveMatrix P2MovRange;
    const Position P1StartPos;
    const Position P2StartPos;
    ImVec2 scrolling;
    bool showGrid;
    int levelOffsetX;
    int levelOffsetY;
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
    GameSettings* gameSettings;

private:
    void init();

    GameMode _mode;
    Turn _turn;
    Board* _board;
    std::vector<NODE_VEC> _nodes;
};
