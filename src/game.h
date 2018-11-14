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
using moveMatrix = bool[8][MRR];
enum GameMode{PvsPC, PCvsPC};

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
    int levelOffsetX{100};
    int levelOffsetY{100};
    int speedMS{500};
    GameMode gameMode;
    bool hacks{false};
    bool debug{false};
    int turnTimes{23};
    bool trails{true};
    bool firstP1{true};
};

struct Traverse {
    double val;
    int ID;
};

struct BackTrack{
    int chldID;
    int parentID;
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


void makeTurns(GameSettings& gs);
inline bool checkWinner(const Node& n){ return n.interspace == 0;}
POS_VEC getPossibleMoves(const Position& p, PlayerIdx turn);
POS_VEC getRanges();
GameSettings setSettings(GameMode mode);


extern Position currPos[PLAYER_COUNT];
extern std::deque<Position> tracers[PLAYER_COUNT];
extern std::vector<NODE_VEC> nodes;
extern PlayerIdx _currPlayer;
extern std::vector<BackTrack> chosenPath;
extern Position lastClicked;
extern bool won;
extern int winnerID;
extern int tt;
extern bool humanClicked;
extern bool firstTurn;


void init(GameSettings gameSettings);
};
