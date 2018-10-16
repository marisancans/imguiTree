#pragma once

//#include "node.h"

const int MRR = 3; // Max movement range
const int MMC = 8; // Max movement count

//enum MovDir // Movement directions
//{
//    UP = 1,
//    UP_RIGHT = 2,
//    RIGHT = 3,
//    DOWN_RIGHT = 4,
//    DOWN = 5,
//    DOWN_LEFT = 6,
//    LEFT = 7,
//    UP_LEFT = 8
//};

// Defines how far a player can go in a direction
struct Movement
{
    int up[MRR];
    int upRight[MRR];
    int right[MRR];
    int downRight[MRR];
    int down[MRR];
    int downLeft[MRR];
    int left[MRR];
    int upLeft[MRR];
};

struct State;
class Board {
public:
    inline Board(int xMax, int yMax, Movement const& P1MovRange,  Movement const& P2MovRange):
        _xMax(xMax), _yMax(yMax), _P1MovRange(P1MovRange), _P2MovRange(P2MovRange) {}
    int* getPossibleMoves(State& state);

private:
    int _xMax;
    int _yMax;
    Movement const& _P1MovRange; // Movement ranges
    Movement const& _P2MovRange;
};
