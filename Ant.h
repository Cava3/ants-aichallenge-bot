#pragma once

#include "AStar.h"
#include "Location.h"

class State;


class Ant
{
    private:
        Location _position;
        Location _target;
        AStar _pathfinder;
        std::vector<Location> _path;

    public:
        Ant(int id, Location location);
        int id;
        Location getPosition();
        Location getTarget();
        void setTarget(const State& state, Location location);
        void playTurn(State& state, double timeLimit);
        int selectDirection(const State& state, double timeLimit);
        Location findClosestFood(const State& state);
        Location takeDecision(const State& state, double timeLimit);
        

};


// TO DO :
// - passer le state en variable globale de la classe
