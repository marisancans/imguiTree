#pragma once

#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "node.h"

const int MRR = 3; // Max movement range

class Node;
struct Position;
using NODE_VEC = std::vector<Node>;
using POS_VEC = std::vector<Position>;
using moveMatrix = int[8][MRR];

// move Matrix column is MovDir enum and index(n) is how far
//[UP][n] [UP_RIGHT][n] [DOWN_RIGHT][n] [DOWN][n] [DOWN_LEFT][n] [LEFT][n] [UP_LEFT][n]


struct GameSettings{
    int maxLayer;
    int maxBoardX;
    int maxBoardY;
    const moveMatrix P1MovRange;
    const moveMatrix P2MovRange;
    const Position P1StartPos;
    const Position P2StartPos;
    ImVec2 scrolling;
    bool showGrid;
    int levelOffsetX;
    int levelOffsetY;
    int speedMS;
};

struct Traverse{
    double val;
    int ID;
};

enum MovDir // Movement directions
{
    UP = 0,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT ,
    LEFT,
    UP_LEFT
};

class Game {
public:
    enum GameMode{PvsPC = 1, PCvsPC =2};
    enum Turn{P1 = 1, P2 = 2};
    Game(GameMode mode, Turn turn, GameSettings gameSettings);
    void genLayer();
    inline const std::vector<NODE_VEC>& getLayers() const { return _nodes; }
    inline void swapTurn(){ _turn = _turn == P1 ? P2 : P1; }
    inline int getNewID(){ return ++_newID; }
    
    void makeTurns();
    bool checkWinner();
    POS_VEC getPossibleMoves(const Position& p, Turn turn) const;
    POS_VEC getRanges() const;
    

    GameSettings gameSettings;
    Node* currNodeP1;
    Node* currNodeP2;
    Turn winner;

private:
    void init();

    GameMode _mode;
    Turn _turn;
    std::vector<NODE_VEC> _nodes;
    int _newID;

};
