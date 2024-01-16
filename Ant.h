#pragma once

#include "AStar.h"
#include "Location.h"

class State;


class Ant
{
    private:
        Location _position;
        Location _nextTurnPosition;
        Location _destination;
        AStar _pathfinder;
        std::vector<Location> _path;
        int _selectDirection(const State& state_ref, Location nLoc, double timeLimit);
        void _setDestination(const State& state_ref, Location location);
        Location _findClosestFood(const State& state_ref);
        void _makeMove(State& state_ref, int direction);

    public:
        Ant(int id, State& state_ref, Location location);
        int id;
        Location getPosition();
        Location getNextTurnPosition();
        Location getDestination();
        void playTurn(State& state_ref, double timeLimit);
        Location takeDecision(const State& state_ref, double timeLimit);
        void validateLastTurnMove(State& state_ref, bool validated);
};


// TO DO :
// - passer le state en variable globale de la classe
