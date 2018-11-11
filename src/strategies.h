// Created on 10/31/2018.
#pragma once
#include <functional>

enum PlayerIdx
{
    P1,
    P2,
    PLAYER_COUNT,
    FIRST = P1,
    NONE
};

static const auto chase = [](auto lhs, auto rhs) { return lhs < rhs; };
static const auto runAway = [](auto lhs, auto rhs) { return lhs > rhs; };

// this is generalizable for any number of players:
//  - if there are multiple players, a player can be a hunter or prey
//  - only thing that changes is interspace calculation function
//    currently it is hard-coded to only look at P1 and P2, but you could
//    iterate through all the players and make each one look for the closest
//    distance from all hunters or something, maybe something else might need
//    to change as well
//  - in case where the strategy cannot depend on just interspace,
//    you can adapt lt and gt to be any kind of function that looks at
//    each node and scores it

// current player strategies:
// P1 = look for shortest distance move (future distance is less than curr distance)
// P2 = look for longest distance move (future distance is greater than curr distance)
static const std::function<bool(double, double)> strategies[PLAYER_COUNT] = { chase, runAway };

inline PlayerIdx nextPlayer(int p)
{
    return ++p == PLAYER_COUNT ? FIRST : PlayerIdx(p);
}
