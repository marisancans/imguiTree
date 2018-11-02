#pragma once

#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "node.h"
#include "deque"

const int MRR = 3; // Max movement range

struct Position;
using NODE_VEC = std::vector<Node>;
using POS_VEC = std::vector<Position>;
using moveMatrix = int[8][MRR];

// move Matrix column is MovDir enum and index(n) is how far
//[UP][n] [NORTH_EAST][n] [DOWN_RIGHT][n] [DOWN][n] [DOWN_LEFT][n] [LEFT][n] [UP_LEFT][n]


struct GameSettings {
    int maxLayer;
    int maxBoardX;
    int maxBoardY;
    moveMatrix movRange[PLAYER_COUNT] = {};
    Position startPos[PLAYER_COUNT] = {};
    ImVec2 scrolling;
    bool showGrid;
    int levelOffsetX;
    int levelOffsetY;
    int speedMS;
};

struct Traverse {
    double val;
    int ID;
};


//
//          NORTH
//            |
//  WEST  ----+----   EAST
//            |
//          SOUTH
//
enum MovDir // Movement directions
{
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST,
};

namespace game {
enum GameMode{PvsPC = 1, PCvsPC =2};

void makeTurns(GameSettings& gs);
bool checkWinner();
POS_VEC getPossibleMoves(const Position& p, PlayerIdx turn);
POS_VEC getRanges();

extern Position currPos[PLAYER_COUNT];
extern std::deque<Position> tracers[PLAYER_COUNT];
extern std::vector<NODE_VEC> nodes;
extern PlayerIdx _currPlayer;
extern POS_VEC chosenPath;

void init(GameMode mode, PlayerIdx turn, GameSettings& gameSettings);
};
