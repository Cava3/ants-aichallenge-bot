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
#include "Food.h"
#include "Location.h"
#include "Square.h"
#include "Timer.h"


/*
    constants
*/
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} }; //{N, E, S, W}

const int PLAYSTYLE_FLEE = 0;
const int PLAYSTYLE_EAT = 1;
const int PLAYSTYLE_ANIHILATE = 2;

/*
    struct to store current state information
*/
struct State
{
    /*
        Variables
        TODO: private and convention
    */
    int rows, cols,
        turn, turns,
        noPlayers,
        antsId, foodId;
    double attackradius, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;
    int64_t seed;
    int playstyle;

    std::vector<std::vector<Square>> grid;
    std::vector<Ant> myAnts;
    // std::vector<Location> myAnts;
    std::vector<Food> food;
    std::vector<Food*> freeFood;
    std::vector<Location> enemyAnts, myHills, enemyHills, waters;

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
    bool isLocationReachable(const Location& location) const;
    bool isLocationSafe(const Location& location);


    void updateVisionInformation();
    void addFromMemory();
    void updateMemory();
    void updatePlaystyle();
    Ant* findAnt(const Location pos, int turn);
    Food* findFood(const Location pos);
    void checkFreeFood();
    void markFood(Food* food_ptr, int antId);
    void deleteAnt(int id);
    void deleteFood(int id);
    void deleteFoodFromFreeFood(Food* food_ptr);
    void deleteMissingFood();
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);
