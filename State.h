#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Ant.h"
#include "Bug.h"
#include "Location.h"
#include "Square.h"
#include "Timer.h"

#define PLAYSTYLE_FLEE 0
#define PLAYSTYLE_EAT 1
#define PLAYSTYLE_ANIHILATE 2

/*
    constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} }; //{N, E, S, W}

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
    */
    int rows, cols,
        turn, turns,
        noPlayers,
        antsId;
    double attackradius, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;
    int64_t seed;
    int playstyle;

    std::vector<std::vector<Square>> grid;
    // std::vector<Ant> myAnts;
    std::vector<Location> myAnts;
    std::vector<Location> enemyAnts, myHills, enemyHills, food, waters;
    Location hills[4];

    Timer timer;
    Bug bug;

    /*
        Functions
    */
    State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);

    double distance(const Location &loc1, const Location &loc2) const;
    Location getLocation(const Location &startLoc, int direction) const;

    void updateVisionInformation();
    void addFromMemory();
    void updateMemory();
    void updatePlaystyle();
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);
