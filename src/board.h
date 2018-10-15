#pragma once

const int MRR = 3; // Max movement range

enum Movement{
    UP = 1,
    UP_RIGHT = 2,
    RIGHT = 3,
    DOWN_RIGHT = 4,
    DOWN = 5,
    DOWN_LEFT = 6,
    LEFT = 7,
    UP_LEFT = 8
};

// Defines how far a player can go in a direction
struct MovementRange{
    int up[MRR];
    int upRight[MRR];
    int right[MRR];
    int downRight[MRR];
    int down[MRR];
    int downLeft[MRR];
    int left[MRR];
    int upLeft[MRR];
};

class Board {
    inline Board(int xMax, int yMax) : _xMax(xMax), _yMax(yMax){}
    void get

private:
    int _xMax;
    int _yMax;
};
